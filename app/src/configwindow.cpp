#include <QCoreApplication>
#include <QCloseEvent>
#include "configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent)
    : QMainWindow(parent), trayIcon(new QSystemTrayIcon(this))
{

    widget = new QWidget(this);
    this->setCentralWidget(widget);

    verticalKeybindLabel = new QLabel(tr("Vertical OCR"));
    verticalKeybindButton = new QPushButton("Alt+Q", widget);
    horizontalKeybindLabel = new QLabel(tr("Horizontal OCR"));
    horizontalKeybindButton = new QPushButton("Alt+A", widget);

    verticalKeybindLabel->setStyleSheet("margin-right: 5px;");
    horizontalKeybindLabel->setStyleSheet("margin-right: 8px;");
    QGridLayout *mainLayout = new QGridLayout(widget);
    mainLayout->addWidget(verticalKeybindLabel, 0, 0);
    mainLayout->addWidget(verticalKeybindButton, 0, 1);
    mainLayout->addWidget(horizontalKeybindLabel, 1, 0);
    mainLayout->addWidget(horizontalKeybindButton, 1, 1);
    setFixedSize(sizeHint());

    QMenu *menu = this->createMenu();
    this->trayIcon->setContextMenu(menu);

    QIcon appIcon = QIcon(":/icons/tray.png");
    this->trayIcon->setIcon(appIcon);
    this->setWindowIcon(appIcon);

    this->trayIcon->show();

    // connect(trayIcon, &QSystemTrayIcon::activated, this, &ConfigWindow::iconActivated);
}

QMenu *ConfigWindow::createMenu()
{
    QAction *quitAction = new QAction("&Quit", this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    QMenu *menu = new QMenu(this);
    menu->addAction(quitAction);

    return menu;
}

void ConfigWindow::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_MACOS
    if (!event->spontaneous() || !isVisible())
    {
        return;
    }
#endif
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

void ConfigWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        show();
        break;
    default:;
    }
}