#include <QClipboard>
#include <QCursor>
#include <QGuiApplication>
#include <QHotkey>
#include <QPainter>
#include <QPixmap>
#include <QStandardPaths>
#include <qdir.h>
#include <sys/stat.h>

#include "utils.h"

QScreen *getActiveScreen() {
    QPoint globalCursorPos = QCursor::pos();
    QScreen *activeScreen = QGuiApplication::screenAt(globalCursorPos);
    return activeScreen;
}

void remove_spaces(char *s) {
    const char *d = s;
    int space_flag = 0;
    int newline_flag = 0;
    do {
        switch (*d) {
        case ' ':
        case '\t':
            if (space_flag) {
                ++d;
                continue;
            }
            space_flag = 1;
            break;
        case '\n':
            if (++newline_flag > 2) {
                // >2 as \n for continuation line and \n\n for new
                // lines.
                ++d;
                continue;
            }
            break;
        default:
            space_flag = 0;
            newline_flag = 0;
        }
    } while (*s++ = *d++);
}

QPixmap grabScreenshot() {
    QScreen *activeScreen = getActiveScreen();
    QPixmap desktopPixmap = QPixmap(activeScreen->geometry().size());
    QPainter p(&desktopPixmap);

    p.drawPixmap(*(new QPoint(0, 0)), activeScreen->grabWindow(0));

    return desktopPixmap;
}

bool pathExist(const char *s) {
    struct stat buffer;
    return (stat(s, &buffer) == 0);
}

QString getTempImage(bool debugFile) {
    QString name = debugFile ? "tempDebugOcrImage.png" : "tempOcrImg.png";
    return QDir::temp().filePath(name);
}

std::string getCoordsFile() {
    return QDir(QStandardPaths::writableLocation(
                    QStandardPaths::GenericCacheLocation))
        .absoluteFilePath("ocrcoords")
        .toStdString();
}

const char *convertToCString(QString src) {
    QByteArray byteArray = src.toUtf8();
    return byteArray.constData();
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    std::vector<std::string> list;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}
