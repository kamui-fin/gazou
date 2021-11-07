#include "ocr.h"
#include <QObject>
#include <QRect>

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
