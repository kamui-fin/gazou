#include "ocr.h"

OCR::OCR()
{
    tess = new tesseract::TessBaseAPI();
    tess->Init("/home/kamui/Coding/Projects/JP_OCR/core/data/models", "jpn_vert", tesseract::OEM_LSTM_ONLY);
    tess->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK_VERT_TEXT);
    tess->SetVariable("tessedit_char_blacklist", "}><L");
    tess->SetVariable("textord_debug_tabfind", "0");
    tess->SetVariable("language_model_ngram_on", "0");
    tess->SetVariable("tessedit_enable_dict_correction", "1");
    tess->SetVariable("textord_really_old_xheight", "1");
    tess->SetVariable("textord_force_make_prop_words", "F");
    tess->SetVariable("edges_max_children_per_outline", "40");
    tess->SetVariable("tosp_threshold_bias2", "1");
    tess->SetVariable("classify_norm_adj_midpointt", "96");
    tess->SetVariable("tessedit_class_miss_scale", "0.002");
    tess->SetVariable("textord_initialx_ile", "1.0");
    tess->SetVariable("preserve_interword_spaces", "1");
    tess->SetVariable("user_defined_dpi", "300");
}

OCR::~OCR()
{
    delete[] result;
    tess->End();
    delete tess;
}

cv::Mat OCR::processImage(char const *path)
{

    cv::Mat image = cv::imread(path, cv::IMREAD_GRAYSCALE);

    int factor = std::max(1, 1800 / image.size().width);
    cv::resize(image, image, cv::Size(), factor, factor, cv::INTER_CUBIC);

    // remove noise and smooth
    cv::Mat filtered;
    cv::adaptiveThreshold(image, filtered, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 41, 3);

    cv::Mat opening;
    cv::morphologyEx(filtered, opening, cv::MORPH_OPEN, {{1}});

    cv::Mat closing;
    cv::morphologyEx(opening, closing, cv::MORPH_CLOSE, {{1}});

    cv::Mat th1;
    cv::threshold(image, th1, 180, 255, cv::THRESH_BINARY);

    cv::Mat th2;
    cv::threshold(th1, th2, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

    cv::Mat blur;
    cv::Size size = {1, 1};
    cv::GaussianBlur(th2, blur, size, 0);

    cv::threshold(blur, image, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

    cv::Mat finalImage;
    cv::bitwise_or(image, closing, finalImage);

    cv::imwrite("/home/kamui/Coding/Projects/JP_OCR/core/data/images/afterProcess.jpg", finalImage);

    cv::cvtColor(finalImage, finalImage, cv::COLOR_GRAY2BGR);

    return finalImage;
}

void OCR::extractText(cv::Mat *img)
{
    tess->SetImage(img->data, img->cols, img->rows, 3, img->step);
    result = tess->GetUTF8Text();
}

char *OCR::ocrImage(char const *path)
{
    cv::Mat img = processImage(path);
    extractText(&img);
    return result;
}