#include "ocr.h"
#include <leptonica/allheaders.h>
#include <string>
#include <map>
#include <iostream>

#define DEBUG

void debug(std::string msg)
{
  std::cout << msg << std::endl;
}

OCR::OCR()
{
  tess = new tesseract::TessBaseAPI();
}

OCR::~OCR()
{
  delete[] result;
  tess->End();
  delete tess;
  pixDestroy(&image);
}

PIX *OCR::processImage(char const *path)
{
  float factor = 3.5f;
  const int otsuSX = 2000;
  const int otsuSY = 2000;
  const int otsuSmoothX = 0;
  const int otsuSmoothY = 0;
  const float otsuScorefract = 0.0f;

  const int usmHalfwidth = 5;
  const float usmFract = 2.5f;

  PIX *pixs = pixRead(path);

  // Convert to grayscale
  pixs = pixConvertRGBToGray(pixs, 0.0, 0.0, 0.0);

  // Resize
  pixs = pixScale(pixs, factor, factor);

  pixs = pixUnsharpMaskingGray(pixs, usmHalfwidth, usmFract);
  pixOtsuAdaptiveThreshold(pixs, otsuSX, otsuSY, otsuSmoothX, otsuSmoothY, otsuScorefract, nullptr, &pixs);
  pixs = pixSelectBySize(pixs, 3, 3, 8,
                         L_SELECT_IF_EITHER, L_SELECT_IF_GT, nullptr);

  // Decide if image needs to be inverted or not
  float pixelAvg = pixAverageOnLine(pixs, 0, 0, pixs->w - 1, 0, 1);
  pixelAvg += pixAverageOnLine(pixs, 0, pixs->h - 1, pixs->w - 1, pixs->h - 1, 1);
  pixelAvg += pixAverageOnLine(pixs, 0, 0, 0, pixs->h - 1, 1);
  pixelAvg += pixAverageOnLine(pixs, pixs->w - 1, 0, pixs->w - 1, pixs->h - 1, 1);
  pixelAvg /= 4.0f;

  if (pixelAvg > .7)
  {
    pixs = pixInvert(pixs, pixs);
  }

  // Add a border
  pixs = pixAddBlackOrWhiteBorder(pixs, 10, 10, 10, 10, L_GET_WHITE_VAL);

#ifdef DEBUG
  pixWrite("core/data/images/afterProcess.png", pixs, IFF_PNG);
#endif
  return pixs;
}

void OCR::extractText()
{
  tess->SetImage(image);
  result = tess->GetUTF8Text();
}

char *OCR::ocrImage(char const *path, ORIENTATION orn)
{
  if (orn != orientation)
  {
    this->setLanguage(orn);
  }

  image = processImage(path);
  extractText();
  postProcessText();
  return result;
}

void OCR::setLanguage(ORIENTATION orn)
{
  const char *lang = orn ? "jpn_vert" : "jpn";
  tess->Init("core/data/models", lang,
             tesseract::OEM_LSTM_ONLY);
  this->setJapaneseParams();

  if (orn == VERTICAL)
  {
    tess->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK_VERT_TEXT);
  }
  else if (orn == HORIZONTAL)
  {
    tess->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
  }

  orientation = orn;
}
void OCR::setJapaneseParams()
{
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

std::map<std::string, std::string> REPLACE_CHARS_VERT = {
    {" ", ""},
    {"|", "ー"},
};
std::map<std::string, std::string> REPLACE_CHARS_HORIZONTAL = {};

void replaceAll(std::string &str, const std::string &from, const std::string &to)
{
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos)
  {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
}

void OCR::postProcessText()
{
  std::string cleanedText = result;
  if (orientation == VERTICAL)
  {
    for (std::map<std::string, std::string>::iterator it = REPLACE_CHARS_VERT.begin(); it != REPLACE_CHARS_VERT.end(); it++)
    {
      replaceAll(cleanedText, it->first, it->second);
    }
  }

  strcpy(result, cleanedText.c_str());
}
