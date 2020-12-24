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
    OCR(char const *modelDir);
    ~OCR();
    char *ocrImage(char const *path, ORIENTATION orn);

private:
    tesseract::TessBaseAPI *tess;
    ORIENTATION orientation = NONE;
    char const *modelFolder;
    char *result;
    PIX *image;

    PIX *processImage(char const *path);
    void extractText();
    void setLanguage(ORIENTATION orn);
    void setJapaneseParams();
    void postProcessText();
};