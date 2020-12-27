#include <QCursor>
#include <QGuiApplication>

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