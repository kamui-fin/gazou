#include "utils.h"
#include "request.h"
#include <QClipboard>
#include <QCursor>
#include <QDir>
#include <QGuiApplication>
#include <QPainter>
#include <QPixmap>
#include <QProcessEnvironment>
#include <QStandardPaths>
#include <QUrl>
#include <QUuid>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <qdir.h>
#include <sys/stat.h>

enum WM { GNOME, KDE, OTHER, SWAY };

bool waylandDetected() {
    auto e = QProcessEnvironment::systemEnvironment();
    QString XDG_SESSION_TYPE = e.value(QStringLiteral("XDG_SESSION_TYPE"));
    QString WAYLAND_DISPLAY = e.value(QStringLiteral("WAYLAND_DISPLAY"));
    return XDG_SESSION_TYPE == QLatin1String("wayland") ||
           WAYLAND_DISPLAY.contains(QLatin1String("wayland"),
                                    Qt::CaseInsensitive);
}

WM getWM() {
    auto e = QProcessEnvironment::systemEnvironment();
    QString XDG_CURRENT_DESKTOP =
        e.value(QStringLiteral("XDG_CURRENT_DESKTOP"));
    QString KDE_FULL_SESSION = e.value(QStringLiteral("KDE_FULL_SESSION"));
    QString GNOME_DESKTOP_SESSION_ID =
        e.value(QStringLiteral("GNOME_DESKTOP_SESSION_ID"));
    WM res = WM::OTHER;
    QStringList desktops = XDG_CURRENT_DESKTOP.split(QChar(':'));
    for (auto &desktop : desktops) {
        if (desktop.contains(QLatin1String("GNOME"), Qt::CaseInsensitive)) {
            return WM::GNOME;
        }
        if (desktop.contains(QLatin1String("sway"), Qt::CaseInsensitive)) {
            return WM::SWAY;
        }
        if (desktop.contains(QLatin1String("kde-plasma"))) {
            return WM::KDE;
        }
    }

    if (!GNOME_DESKTOP_SESSION_ID.isEmpty()) {
        return WM::GNOME;
    }

    if (!KDE_FULL_SESSION.isEmpty()) {
        return WM::KDE;
    }

    return res;
}

void freeDesktopPortal(bool &ok, QPixmap &res) {
    QDBusInterface screenshotInterface(
        QStringLiteral("org.freedesktop.portal.Desktop"),
        QStringLiteral("/org/freedesktop/portal/desktop"),
        QStringLiteral("org.freedesktop.portal.Screenshot"));

    // unique token
    QString token =
        QUuid::createUuid().toString().remove('-').remove('{').remove('}');

    // premake interface
    auto *request = new OrgFreedesktopPortalRequestInterface(
        QStringLiteral("org.freedesktop.portal.Desktop"),
        "/org/freedesktop/portal/desktop/request/" +
            QDBusConnection::sessionBus().baseService().remove(':').replace(
                '.', '_') +
            "/" + token,
        QDBusConnection::sessionBus());

    QEventLoop loop;
    const auto gotSignal = [&res, &loop](uint status, const QVariantMap &map) {
        if (status == 0) {
            // Parse this as URI to handle unicode properly
            QUrl uri = map.value("uri").toString();
            QString uriString = uri.toLocalFile();
            res = QPixmap(uriString);
            res.setDevicePixelRatio(qApp->devicePixelRatio());
            QFile imgFile(uriString);
            imgFile.remove();
        }
        loop.quit();
    };

    // prevent racy situations and listen before calling screenshot
    QMetaObject::Connection conn = QObject::connect(
        request, &org::freedesktop::portal::Request::Response, gotSignal);

    screenshotInterface.call(
        QStringLiteral("Screenshot"), "",
        QMap<QString, QVariant>({{"handle_token", QVariant(token)},
                                 {"interactive", QVariant(false)}}));

    loop.exec();
    QObject::disconnect(conn);
    request->Close().waitForFinished();
    request->deleteLater();

    if (res.isNull()) {
        ok = false;
    }
}

QPixmap grabScreenshot(bool &ok) {
    if (waylandDetected()) {
        QPixmap res;
        // handle screenshot based on DE
        switch (getWM()) {
        case WM::GNOME:
        case WM::KDE:
        case WM::SWAY: {
            freeDesktopPortal(ok, res);
            break;
        }
        default:
            ok = false;
            break;
        }
        return res;
    } else {
        QScreen *activeScreen = getActiveScreen();
        QPixmap desktopPixmap = QPixmap(activeScreen->geometry().size());
        QPainter p(&desktopPixmap);
        p.drawPixmap(*(new QPoint(0, 0)), activeScreen->grabWindow(0));
        return desktopPixmap;
    }
}

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

int get_char(char *input, int offset, char buff[5]) {
    input += offset;
    int size = UTF8_CHAR_LEN(*input);
    memcpy(buff, input, size);
    buff[size] = '\0';
    return size;
}

bool testRange(char text[5], unsigned int from, unsigned int to) {
    int i = 0;
    unsigned int letter = text[i];
    while (letter != '\0') {
        if (letter < from || letter > to) {
            return false;
        }
        i += 1;
        letter = text[i];
    }
    return true;
}

bool isJapanese(char text[5]) {
    return (isKanji(text) || isHiragana(text) || isKatakana(text));
}

bool isKanji(char text[5]) {
    return testRange(text, 0x4E00, 0x9FAF);
}

bool isHiragana(char text[5]) {
    return testRange(text, 0x3040, 0x309F);
}

bool isKatakana(char text[5]) {
    return testRange(text, 0x30A0, 0x30FF);
}
