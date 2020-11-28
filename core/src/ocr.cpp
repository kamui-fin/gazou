#include "ocr.h"

OCR::OCR()
{
    tess = new tesseract::TessBaseAPI();
    tess->Init(NULL, "jpn_vert", tesseract::OEM_LSTM_ONLY);
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
}

OCR::~OCR()
{
    delete[] result;
    tess->End();
    delete tess;
}

cv::Mat OCR::processImage(char const *path)
{

    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    // cv::resize(image, image, cv::Size(), 1.2, 1.2, cv::INTER_CUBIC);
    // cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    // cv::dilate(image, image, cv::Mat());
    // cv::erode(image, image, cv::Mat());
    // cv::medianBlur(image, image, 3);
    // cv::adaptiveThreshold(image, image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 31, 2);
    cv::imwrite("/home/kamui/Coding/Projects/JP_OCR/core/data/images/afterProcess.jpg", image);
    return image;
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