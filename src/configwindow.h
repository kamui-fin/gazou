#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QHotkey>
#include <QSettings>
#include <QPushButton>
#include <QLabel>
#include <vector>

class ConfigWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConfigWindow(std::map<std::string, QHotkey *> hotkeys, QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QWidget *widget;
    std::map<std::string, QHotkey *> hotkeys;
    QLabel *verticalKeybindLabel;
    QPushButton *verticalKeybindButton;

    QLabel *horizontalKeybindLabel;
    QPushButton *horizontalKeybindButton;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QSettings *settings;

    QMenu *createMenu();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void handleHotkeyButton();
};