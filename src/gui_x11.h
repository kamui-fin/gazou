#include "ocr.h"
#include <QApplication>
#include <QClipboard>
#include <QHotkey>
#include <QString>

void setRegistered(std::map<std::string, QHotkey *> hotkeys, bool registered);

QHotkey *setupOCRHotkey(QString sequence, char *callback(ORIENTATION orn),
                        ORIENTATION orn = NONE);

void startGui(QApplication *app, char *interactive(ORIENTATION orn),
              char *prevOcr(ORIENTATION orn));
