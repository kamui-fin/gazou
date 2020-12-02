#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>

class ConfigWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConfigWindow(QWidget *parent = 0);

private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QMenu *createMenu();
};