#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>

class ConfigWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConfigWindow(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QMenu *createMenu();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
};