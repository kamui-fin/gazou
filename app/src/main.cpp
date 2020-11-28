#include <iostream>
#include <QApplication>
#include <QDebug>
#include <QHotkey>
#include <QClipboard>
#include <bits/stdc++.h>
#include <string.h>
#include "ocr.h"

void copyToClipboard(char *text, QClipboard *cb)
{
    cb->setText(text);
}

QHotkey *setupHotkey(char const *sequence, void callback())
{
    QHotkey *hotkey = new QHotkey(QKeySequence(sequence), true, qApp);
    QObject::connect(hotkey, &QHotkey::activated, qApp,
                     callback);
    return hotkey;
}

void runOCR()
{
    static OCR *ocr = new OCR();
    // screenshot and save to temp.png
    const char *imagePath = "/home/kamui/Coding/Projects/JP_OCR/core/data/images/temp.png";
    const char *command = "maim -s -u ";
    std::string strC = command;
    strC += imagePath;
    system(strC.c_str());

    char *result = ocr->ocrImage(imagePath);
    std::cout << result << std::endl;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QClipboard *clipboard = QApplication::clipboard();

    char const *verticalHotkey = "alt+q";
    setupHotkey(verticalHotkey, runOCR);

    return app.exec();
}
