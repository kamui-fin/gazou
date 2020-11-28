#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>

class OCR
{
public:
    OCR();
    ~OCR();
    char *ocrImage(char const *path);

private:
    tesseract::TessBaseAPI *tess;
    char *result;

    cv::Mat processImage(char const *path);
    void extractText(cv::Mat *img);
};