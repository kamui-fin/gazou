#include "utils.h"
#include <QCursor>
#include <QGuiApplication>

QScreen *getActiveScreen()
{
    QPoint globalCursorPos = QCursor::pos();
    QScreen *activeScreen = QGuiApplication::screenAt(globalCursorPos);
    return activeScreen;
}