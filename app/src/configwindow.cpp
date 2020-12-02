#include <QCoreApplication>
#include "configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent)
    : QMainWindow(parent), trayIcon(new QSystemTrayIcon(this))
{
    QMenu *menu = this->createMenu();
    this->trayIcon->setContextMenu(menu);

    QIcon appIcon = QIcon(":/icons/tray.png");
    this->trayIcon->setIcon(appIcon);
    this->setWindowIcon(appIcon);

    this->trayIcon->show();
}

QMenu *ConfigWindow::createMenu()
{
    QAction *quitAction = new QAction("&Quit", this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    QMenu *menu = new QMenu(this);
    menu->addAction(quitAction);

    return menu;
}
