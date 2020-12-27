#include <QApplication>
#include <QHotkey>
#include <QObject>
#include <QClipboard>
#include <vector>
#include <map>

#include "ocr.h"
#include "configwindow.h"
#include "selectorwidget.h"
#include "utils.h"
#include "config.h"


void copyToClipboard(char *text, QClipboard *cb)
{
    cb->setText(text);
}

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

    const char *imagePath = "/tmp/tempImg.png";

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
    QApplication app(argc, argv);
    QSettings settings("gazou", "gazou");

    QString verticalHotkey = settings.value("Hotkeys/verticalOCR", "Alt+A").toString();
    QString horizontalHotkey = settings.value("Hotkeys/horizontalOCR", "Alt+D").toString();

    QHotkey *vKey = setupOCRHotkey(verticalHotkey, runOCR, VERTICAL);
    QHotkey *hKey = setupOCRHotkey(horizontalHotkey, runOCR, HORIZONTAL);

    std::map<std::string, QHotkey *> hotkeys = {
        {"verticalOCR", vKey},
        {"horizontalOCR", hKey},
    };

    ConfigWindow *cw = new ConfigWindow(hotkeys);

    app.setQuitOnLastWindowClosed(false);
    return app.exec();
}
