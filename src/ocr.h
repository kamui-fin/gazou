#include <QString>

#include <leptonica/allheaders.h>
#include <map>
#include <tesseract/baseapi.h>

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

  PIX *processImage(QString path);
  void extractText();
  void setLanguage(ORIENTATION orn);
  void setJapaneseParams();
};