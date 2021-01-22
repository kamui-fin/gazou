#include <QApplication>
#include <QHotkey>
#include <QObject>
#include <QClipboard>
#include <QDir>
#include <vector>
#include <map>

#include "ocr.h"
#include "configwindow.h"
#include "selectorwidget.h"
#include "utils.h"
#include "config.h"

#include <iostream>

bool prev_HORIZONTAL {true};

QHotkey *setupOCRHotkey(QString sequence, void callback(ORIENTATION orn), ORIENTATION orn)
{
    QHotkey *hotkey = new QHotkey(QKeySequence(sequence), true, qApp);
    QObject::connect(hotkey, &QHotkey::activated, qApp, [=]() {
        callback(orn);
    });
    return hotkey;
}

void runOCR(ORIENTATION orn)
{
    static OCR *ocr = new OCR();
    static QClipboard *clipboard = QApplication::clipboard();

    QString imagePath = QDir::tempPath().append(QDir::separator()).append("tempImg.png");

    SelectorWidget sw;
    sw.exec();

    char *result = ocr->ocrImage(imagePath, orn);
    clipboard->setText(result);

    #ifdef DEBUG
        qDebug(result);
    #endif
}


int main(int argc, char **argv)
{
    std::cout << prev_HORIZONTAL;
    QApplication app(argc, argv);
    QSettings settings("gazou", "gazou");

    QString verticalHotkey = settings.value("Hotkeys/verticalOCR", "Alt+A").toString();
    QString horizontalHotkey = settings.value("Hotkeys/horizontalOCR", "Alt+D").toString();
    QString repeatHotkey = settings.value("Hotkeys/repeatOCR", "Alt+F").toString();

    QHotkey *vKey = setupOCRHotkey(verticalHotkey, runOCR, VERTICAL);
    QHotkey *hKey = setupOCRHotkey(horizontalHotkey, runOCR, HORIZONTAL);
    QHotkey *rKey = setupOCRHotkey(repeatHotkey, runOCR, REPEAT);

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
