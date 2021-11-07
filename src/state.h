#include <QObject>
#include <QRect>
#include "ocr.h"

struct LastOCRInfo {
    ORIENTATION orn;
    QRect rect;
};

class State : public QObject {
    Q_OBJECT

  public:
    State();

    bool getCurrentlySelecting();
    void setCurrentlySelecting(bool curSel);

    LastOCRInfo getLastOCRInfo();
    void setLastOCRInfo(LastOCRInfo inf);

  private:
    bool currentlySelecting;
    LastOCRInfo lastInfo;
};
