#include "gui_x11.h"
#include "configwindow.h"
#include <QApplication>
#include <QClipboard>
#include <QHotkey>

void setRegistered(std::map<std::string, QHotkey *> hotkeys, bool registered) {
    for (auto const &x : hotkeys) {
        (x.second)->setRegistered(registered);
    }
}

QHotkey *setupOCRHotkey(QString sequence, void callback(ORIENTATION orn),
                        ORIENTATION orn) {
    QHotkey *hotkey = new QHotkey(QKeySequence(sequence), true, qApp);
    QObject::connect(hotkey, &QHotkey::activated, qApp,
                     [=]() { callback(orn); });
    return hotkey;
}

int startGui(int argc, char **argv, QClipboard *clipboard,
             void runRegOCR(ORIENTATION orn),
             void runPrevOCR(ORIENTATION orn)) {
    QApplication app(argc, argv);
    clipboard = QApplication::clipboard();
    QSettings settings("gazou", "gazou");

    QString verticalHotkey =
        settings.value("Hotkeys/verticalOCR", "Alt+A").toString();
    QString horizontalHotkey =
        settings.value("Hotkeys/horizontalOCR", "Alt+D").toString();
    QString prevOCRHotkey =
        settings.value("Hotkeys/repeatOCR", "Alt+S").toString();

    QHotkey *vKey = setupOCRHotkey(verticalHotkey, runRegOCR, VERTICAL);
    QHotkey *hKey = setupOCRHotkey(horizontalHotkey, runRegOCR, HORIZONTAL);
    QHotkey *prevKey = setupOCRHotkey(prevOCRHotkey, runPrevOCR);

    std::map<std::string, QHotkey *> hotkeys = {
        {"verticalOCR", vKey}, {"horizontalOCR", hKey}, {"repeatOCR", prevKey}};

    new ConfigWindow(hotkeys);

    app.setQuitOnLastWindowClosed(false);
    int ret = app.exec();
    setRegistered(hotkeys, false);
    return ret;
}
