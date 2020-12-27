#include <QString>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <map>

enum ORIENTATION
{
    HORIZONTAL,
    VERTICAL,
    NONE,
};

class OCR
{
public:
    OCR();
    ~OCR();
    char *ocrImage(QString path, ORIENTATION orn);

private:
    tesseract::TessBaseAPI *tess;
    ORIENTATION orientation = NONE;
    char *result;
    PIX *image;

    PIX *processImage(QString path);
    void extractText();
    void setLanguage(ORIENTATION orn);
    void setJapaneseParams();
};