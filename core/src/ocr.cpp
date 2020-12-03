#include "ocr.h"

OCR::OCR() {
  tess = new tesseract::TessBaseAPI();
  this->setLanguage();
}

OCR::~OCR() {
  delete[] result;
  tess->End();
  delete tess;
}

cv::Mat OCR::processImage(char const *path) {

  cv::Mat image = cv::imread(path, 0);

  int factor = std::max(1, 1000 / image.size().width);
  cv::resize(image, image, cv::Size(), factor, factor, cv::INTER_CUBIC);

  cv::InputArray kernel = {{1}};
  cv::dilate(image, image, kernel);
  cv::erode(image, image, kernel);

  cv::Size ksize = {5, 5};
  cv::GaussianBlur(image, image, ksize, 0);

  cv::threshold(image, image, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
  cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
  cv::imwrite(
      "/home/kamui/Coding/Projects/JP_OCR/core/data/images/afterProcess.jpg",
      image);

  return image;
}

void OCR::extractText(cv::Mat *img) {
  tess->SetImage(img->data, img->cols, img->rows, 3, img->step);
  result = tess->GetUTF8Text();
}

char *OCR::ocrImage(char const *path, ORIENTATION orn) {
  cv::Mat img = processImage(path);
  this->setLanguage(orn);
  extractText(&img);
  return result;
}

void OCR::setLanguage(ORIENTATION orn) {
  const char *lang = orn ? "jpn_vert" : "jpn";
  if (orn == VERTICAL) {
    tess->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK_VERT_TEXT);
  } else {
    tess->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
  }

  tess->Init("/home/kamui/Coding/Projects/JP_OCR/core/data/models", lang,
             tesseract::OEM_LSTM_ONLY);
  this->setJapaneseParams();
}
void OCR::setJapaneseParams() {
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
