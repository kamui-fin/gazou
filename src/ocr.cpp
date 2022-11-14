#include <QDir>
#include <QString>
#include <iostream>
#include <qdir.h>
#include <utility>
#include <vector>
#include <string.h>

#include "config.h"
#include "ocr.h"
#include "utils.h"

OCR::OCR() {
    tess = new tesseract::TessBaseAPI();
    orientation = NONE;
    corrections = {{"〈", "く"}, {"<", "く"}, {"＜", "く"}, {"L", "し"}, {"Ｌ", "し"}, {"z", "え"}, {"Z", "え"}, {"U", "じ"}, {"ー", ""}, {"一", " "}, {"―", ""}, {"‐", ""}, {"—", ""}, {"－", ""}, {"-", ""}, {"_", ""}, {"|", ""}};
    settings = new QSettings("gazou", "gazou");
}

OCR::~OCR() {
    tess->End();
    delete tess;
    delete[] result;
    pixDestroy(&image);
}

PIX *OCR::processImage(QString path) {
    float factor = 3.5f;
    const int otsuSX = 2000;
    const int otsuSY = 2000;
    const int otsuSmoothX = 0;
    const int otsuSmoothY = 0;
    const float otsuScorefract = 0.0f;

    const int usmHalfwidth = 5;
    const float usmFract = 2.5f;

    QByteArray array = path.toUtf8();
    const char *imageLocation = array.constData();
    PIX *pixs = pixRead(imageLocation);
    if (pixGetDepth(pixs) == 8) {
        // Already Grayscale so no conversion required.
        // FIXME Find a better solution for this, Couldn't find how to
        // check for number of bands with pixmap
    } else {
        // Convert to grayscale
        pixs = pixConvertRGBToGray(pixs, 0.0, 0.0, 0.0);
    }

    // Resize
    pixs = pixScale(pixs, factor, factor);

    pixs = pixUnsharpMaskingGray(pixs, usmHalfwidth, usmFract);
    pixOtsuAdaptiveThreshold(pixs, otsuSX, otsuSY, otsuSmoothX, otsuSmoothY,
                             otsuScorefract, nullptr, &pixs);
    pixs = pixSelectBySize(pixs, 3, 3, 8, L_SELECT_IF_EITHER, L_SELECT_IF_GT,
                           nullptr);

    // Decide if image needs to be inverted or not
    float pixelAvg = pixAverageOnLine(pixs, 0, 0, pixs->w - 1, 0, 1);
    pixelAvg +=
        pixAverageOnLine(pixs, 0, pixs->h - 1, pixs->w - 1, pixs->h - 1, 1);
    pixelAvg += pixAverageOnLine(pixs, 0, 0, 0, pixs->h - 1, 1);
    pixelAvg +=
        pixAverageOnLine(pixs, pixs->w - 1, 0, pixs->w - 1, pixs->h - 1, 1);
    pixelAvg /= 4.0f;

    if (pixelAvg > .7) {
        pixs = pixInvert(pixs, pixs);
    }

    // Add a border
    pixs = pixAddBlackOrWhiteBorder(pixs, 10, 10, 10, 10, L_GET_WHITE_VAL);
#ifdef DEBUG
    QString loc = getTempImage(true);
    pixWrite(convertToCString(loc), pixs, IFF_PNG);
#endif
    return pixs;
}

void OCR::extractText() {
    tess->SetImage(image);
    result = tess->GetUTF8Text();
}

char *OCR::ocrImage(QString path, ORIENTATION orn) {
    this->setLanguage(orn);

    image = processImage(path);
    extractText();

    QString lang = settings->value("language", "jpn").toString();
    if (lang == "jpn")
        postprocess();
    return result;
}

void OCR::setLanguage(ORIENTATION orn) {
    QString lang = settings->value("language", "jpn").toString();

    if (orn == VERTICAL) {
        lang += "_vert";
    }

    tess->Init(GAZOU_MODEL_FOLDER, lang.toLocal8Bit().constData(),
               tesseract::OEM_LSTM_ONLY);

    this->setParams();

    tesseract::PageSegMode pageSeg = orn == VERTICAL
                                         ? tesseract::PSM_SINGLE_BLOCK_VERT_TEXT
                                         : tesseract::PSM_SINGLE_BLOCK;

    tess->SetPageSegMode(pageSeg);

    orientation = orn;
}

void OCR::setParams() {
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
    tess->SetVariable("debug_file", "/dev/null");

    // experimental
    tess->SetVariable("chop_enable", "T");
    tess->SetVariable("use_new_state_cost", "F");
    tess->SetVariable("segment_segcost_rating", "F");
    tess->SetVariable("enable_new_segsearch", "0");
    tess->SetVariable("language_model_ngram_on", "0");
    tess->SetVariable("textord_force_make_prop_words", "F");
    tess->SetVariable("edges_max_children_per_outline", "40");
}

void OCR::postprocess() {
    char curr[5];
    char prev[5];
    char next[5];
    size_t strsize = 0;
    get_char(result, strsize, curr);

    for (size_t offset = 0; offset <= strlen(result); offset += strsize) {
        strsize = get_char(result, offset + strlen(curr), next);
        correctCommonMistake(curr, "く", prev, next);
        correctCommonMistake(curr, "し", prev, next);
        correctCommonMistake(curr, "じ", prev, next);
        correctCommonMistake(curr, "え", prev, next);
        correctCommonMistake(curr, "、", prev, next);
        correctCommonMistake(curr, "。", prev, next);

        correctKanjiOne(curr, next);
        correctKatakanaDash(curr, prev);

        strncpy(prev, curr, 5);
        strncpy(curr, next, 5);
    }
    remove_spaces(result);
}

void OCR::correctCommonMistake(char input[5], char repl[4], char prev[5],
                               char next[5]) {
    char* substitute = corrections[input];
    if (substitute == NULL) {
        return;
    }
    if (strcmp(substitute, repl) == 0) {
        if ((strlen(prev) != 0 && isJapanese(prev)) ||
            (strlen(next) != 0 && isJapanese(next))) {
            strncpy(input, repl, 5);
        }
    }
}

void OCR::correctKatakanaDash(char input[5], char prev[5]) {
    if (corrections.count(input) > 0) {
        if (strlen(prev) != 0 && isKatakana(prev)) {
            strncpy(input, "ー", 5);
        }
    }
}
void OCR::correctKanjiOne(char input[5], char next[5]) {
    if (corrections.count(input) > 0) {
        if (strlen(next) != 0) {
            if (isKanji(next) || isHiragana(next)) {
                strncpy(input, "一", 5);
            }
        }
    }
}
