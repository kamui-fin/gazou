#ifndef OCR_H_
#define OCR_H_

#include <QString>
#include <QSettings>
#include <leptonica/allheaders.h>
#include <map>
#include <tesseract/baseapi.h>
#include <vector>

enum ORIENTATION {
    HORIZONTAL,
    VERTICAL,
    NONE,
};

class OCR {
  public:
    OCR();
    ~OCR();
    char *ocrImage(QString path, ORIENTATION orn);

  private:
    tesseract::TessBaseAPI *tess;
    ORIENTATION orientation;
    QString lang;
    char *result;
    PIX *image;
    std::map<char *, char*> corrections;
    QSettings *settings;

    PIX *processImage(QString path);
    void extractText();
    void setLanguage(ORIENTATION orn);
    void setParams();
    // text processing
    void postprocess();
    void correctCommonMistake(char input[5], char repl[4], char prev[5],
                              char next[5]);
    void correctKatakanaDash(char input[5], char prev[5]);
    void correctKanjiOne(char input[5], char next[5]);
};

#endif
