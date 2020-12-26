#include <QApplication>
#include <QDebug>
#include <QHotkey>
#include <QObject>
#include <QClipboard>
#include <QSignalMapper>
#include <QObject>
#include <QEventLoop>
#include <QtWidgets>
#include <QDir>

#include <bits/stdc++.h>
#include <string.h>
#include <iostream>
#include <vector>

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
    const char *imagePath = "/tmp/tempImg.png";

    static OCR *ocr = new OCR();
    static QClipboard *clipboard = QApplication::clipboard();
    SelectorWidget sw;
    sw.exec();

    char *result = ocr->ocrImage(imagePath, orn);
    qDebug() << result;
    clipboard->setText(result);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QSettings settings("gazou", "gazou");

    QString verticalHotkey = settings.value("Hotkeys/verticalOCR", "Alt+A").toString();
    QString horizontalHotkey = settings.value("Hotkeys/horizontalOCR", "Alt+D").toString();

    QHotkey *vKey = setupOCRHotkey(verticalHotkey, runOCR, VERTICAL);
    QHotkey *hKey = setupOCRHotkey(horizontalHotkey, runOCR, HORIZONTAL);

    std::vector<QHotkey *> hotkeys = {vKey, hKey};

    ConfigWindow *cw = new ConfigWindow(hotkeys);

    app.setQuitOnLastWindowClosed(false);
    return app.exec();
}
