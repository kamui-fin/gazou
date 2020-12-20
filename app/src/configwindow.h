#include <QMainWindow>
#include <QSystemTrayIcon>
#include <vector>
#include <QHotkey>
#include <QMenu>
#include <QtWidgets>

class ConfigWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConfigWindow(std::vector<QHotkey *>, QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QWidget *widget;
    std::vector<QHotkey *> hotkeys;
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