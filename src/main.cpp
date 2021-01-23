#include "main.h"
#include "ocr.h"
#include "configwindow.h"
#include "selectorwidget.h"
#include "utils.h"
#include "config.h"

QHotkey *setupOCRHotkey(
    QString sequence, void callback(ORIENTATION orn, bool prevHor), ORIENTATION orn, bool prevHor)
{
    QHotkey *hotkey = new QHotkey(QKeySequence(sequence), true, qApp);
    QObject::connect(hotkey, &QHotkey::activated, qApp, [=]() {
        callback(orn,prevHor);
    });
    return hotkey;
}

void runOCR(ORIENTATION orn, bool prevHor)
{
    static OCR *ocr = new OCR();
    static QClipboard *clipboard = QApplication::clipboard();

    QString imagePath = QDir::tempPath().append(QDir::separator()).append("tempImg.png");

    SelectorWidget sw;
    if(orn==REPEAT){
        orn = prevHor ? HORIZONTAL : VERTICAL;
        if(sw.savedRect.topLeft()!=constants::zero && sw.savedRect.bottomRight()!=constants::zero){
            sw.grabUsingSavedRect();
        }
        else{
            sw.exec();
        }
    }
    else{
        sw.exec();
    }
    prevHor = orn==HORIZONTAL ? true : false;

    char *result = ocr->ocrImage(imagePath, orn);
    clipboard->setText(result);

    #ifdef DEBUG
        qDebug(result);
    #endif
}


int main(int argc, char **argv)
{
    bool prevHor = true;

    QApplication app(argc, argv);
    QSettings settings("gazou", "gazou");

    QString verticalHotkey = settings.value("Hotkeys/verticalOCR", "Alt+A").toString();
    QString horizontalHotkey = settings.value("Hotkeys/horizontalOCR", "Alt+D").toString();
    QString repeatHotkey = settings.value("Hotkeys/repeatOCR", "Alt+F").toString();

    QHotkey *vKey = setupOCRHotkey(verticalHotkey, runOCR, VERTICAL, prevHor);
    QHotkey *hKey = setupOCRHotkey(horizontalHotkey, runOCR, HORIZONTAL, prevHor);
    QHotkey *rKey = setupOCRHotkey(repeatHotkey, runOCR, REPEAT, prevHor);

    std::map<std::string, QHotkey *> hotkeys = {
        {"verticalOCR", vKey},
        {"horizontalOCR", hKey},
        {"repeatOCR", rKey}
    };

    ConfigWindow *cw = new ConfigWindow(hotkeys);

    app.setQuitOnLastWindowClosed(false);
    int ret = app.exec();
    setRegistered(hotkeys, false);
    return ret;
}
