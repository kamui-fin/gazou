#include <QApplication>
#include <QDebug>
#include <QHotkey>
#include <QClipboard>
#include <QSignalMapper>
#include <QObject>
#include <QEventLoop>
#include <QtWidgets>

#include <bits/stdc++.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "ocr.h"
#include "configwindow.h"
#include "selectorwidget.h"

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
    static SelectorWidget sw;
    static QClipboard *clipboard = QApplication::clipboard();
    // screenshot and save to temp.png
    const char *imagePath = "core/data/images/temp.png";
    // sw.exec();

    const char *command = "sleep 0.2 ; scrot -s -z -f -o ";
    std::string strC = command;
    strC += imagePath;
    system(strC.c_str());

    char *result = ocr->ocrImage(imagePath, orn);
    std::cout << result << std::endl;
    clipboard->setText(result);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QSettings settings("JP OCR", "jpocr");

    QString verticalHotkey = settings.value("Hotkeys/verticalOCR", "Alt+A").toString();
    QString horizontalHotkey = settings.value("Hotkeys/horizontalOCR", "Alt+D").toString();

    QHotkey *vKey = setupOCRHotkey(verticalHotkey, runOCR, VERTICAL);
    QHotkey *hKey = setupOCRHotkey(horizontalHotkey, runOCR, HORIZONTAL);

    std::vector<QHotkey *> hotkeys = {vKey, hKey};

    ConfigWindow *cw = new ConfigWindow(hotkeys);
    cw->show();

    return app.exec();
}
