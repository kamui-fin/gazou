#include "ocr.h"
#include <QClipboard>
#include <QHotkey>
#include <QString>

void setRegistered(std::map<std::string, QHotkey *> hotkeys, bool registered);

QHotkey *setupOCRHotkey(QString sequence, void callback(ORIENTATION orn),
                        ORIENTATION orn = NONE);

int startGui(int argc, char **argv, QClipboard *clipboard,
             void runRegOCR(ORIENTATION orn), void runPrevOCR(ORIENTATION orn));
