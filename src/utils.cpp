#include <QCursor>
#include <QGuiApplication>
#include <QHotkey>
#include <QClipboard>

#include "utils.h"

QScreen *getActiveScreen()
{
    QPoint globalCursorPos = QCursor::pos();
    QScreen *activeScreen = QGuiApplication::screenAt(globalCursorPos);
    return activeScreen;
}


void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

void setRegistered(std::map<std::string, QHotkey *> hotkeys, bool registered)
{
    for (auto const& x : hotkeys)
    {
            (x.second)->setRegistered(registered);
    }
}

void copyToClipboard(char *text, QClipboard *cb)
{
    cb->setText(text);
}