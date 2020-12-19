#include <QApplication>
#include <QDebug>
#include <QHotkey>
#include <QClipboard>
#include <QSignalMapper>
#include <QtWidgets>

#include <bits/stdc++.h>
#include <string.h>
#include <iostream>

#include "ocr.h"
#include "configwindow.h"

void copyToClipboard(char *text, QClipboard *cb)
{
    cb->setText(text);
}

QHotkey *setupOCRHotkey(char const *sequence, void callback(ORIENTATION orn), ORIENTATION orn)
{
    QHotkey *hotkey = new QHotkey(QKeySequence(sequence), true, qApp);
    QSignalMapper *signalMapper = new QSignalMapper(qApp);
    QObject::connect(hotkey, &QHotkey::activated, qApp, [=]() {
        callback(orn);
    });
    return hotkey;
}

void runOCR(ORIENTATION orn)
{
    static OCR *ocr = new OCR();
    static QClipboard *clipboard = QApplication::clipboard();
    // screenshot and save to temp.png
    const char *imagePath = "core/data/images/temp.png";
    const char *command = "maim -s -u ";
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

    char const *verticalHotkey = "alt+q";
    char const *horizontalHotkey = "alt+a";
    setupOCRHotkey(verticalHotkey, runOCR, VERTICAL);
    setupOCRHotkey(horizontalHotkey, runOCR, HORIZONTAL);

    ConfigWindow w;

    return app.exec();
}
