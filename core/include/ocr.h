#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>

enum ORIENTATION
{
    HORIZONTAL,
    VERTICAL
};

class OCR
{
public:
    OCR();
    ~OCR();
    char *ocrImage(char const *path, ORIENTATION orn);

private:
    tesseract::TessBaseAPI *tess;
    char *result;

    cv::Mat processImage(char const *path);
    void extractText(cv::Mat *img);
    void setLanguage(ORIENTATION orn = VERTICAL);
    void setJapaneseParams();
};