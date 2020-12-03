#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QtWidgets>

class ConfigWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConfigWindow(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QWidget *widget;

    QLabel *verticalKeybindLabel;
    QPushButton *verticalKeybindButton;

    QLabel *horizontalKeybindLabel;
    QPushButton *horizontalKeybindButton;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QMenu *createMenu();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
};