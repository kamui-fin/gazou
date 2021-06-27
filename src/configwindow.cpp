#include <QAction>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>

#include "configwindow.h"
#include "keydialog.h"
#include "utils.h"

ConfigWindow::ConfigWindow(std::map<std::string, QHotkey *> hotkeys,
                           QWidget *parent)
    : QMainWindow(parent), trayIcon(new QSystemTrayIcon(this)) {

    this->hotkeys = hotkeys;
    settings = new QSettings("gazou", "gazou");
    widget = new QWidget(this);
    this->setCentralWidget(widget);

    verticalKeybindButton = new QPushButton(
        settings->value("Hotkeys/verticalOCR", "Alt+A").toString(), widget);
    horizontalKeybindButton = new QPushButton(
        settings->value("Hotkeys/horizontalOCR", "Alt+D").toString(), widget);
    lastOCRKeybindButton = new QPushButton(
        settings->value("Hotkeys/previousOCR", "Alt+S").toString(), widget);

    verticalKeybindLabel = new QLabel(tr("Vertical OCR"));
    horizontalKeybindLabel = new QLabel(tr("Horizontal OCR"));
    lastOCRKeybindLabel = new QLabel(tr("Repeat OCR"));

    verticalKeybindLabel->setStyleSheet("margin-right: 5px;");
    horizontalKeybindLabel->setStyleSheet("margin-right: 8px;");
    lastOCRKeybindLabel->setStyleSheet("margin-right: 8px;");

    verticalKeybindButton->setObjectName("verticalOCR");
    horizontalKeybindButton->setObjectName("horizontalOCR");
    lastOCRKeybindButton->setObjectName("repeatOCR");

    QGridLayout *mainLayout = new QGridLayout(widget);
    mainLayout->addWidget(verticalKeybindLabel, 0, 0);
    mainLayout->addWidget(verticalKeybindButton, 0, 1);
    mainLayout->addWidget(horizontalKeybindLabel, 1, 0);
    mainLayout->addWidget(horizontalKeybindButton, 1, 1);
    mainLayout->addWidget(lastOCRKeybindLabel, 2, 0);
    mainLayout->addWidget(lastOCRKeybindButton, 2, 1);

    setFixedSize(sizeHint());

    QMenu *menu = this->createMenu();
    this->trayIcon->setContextMenu(menu);

    QIcon appIcon = QIcon(":/tray.png");
    this->trayIcon->setIcon(appIcon);
    this->setWindowIcon(appIcon);

    this->trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this,
            &ConfigWindow::iconActivated);
    connect(verticalKeybindButton, &QPushButton::clicked, this,
            &ConfigWindow::handleHotkeyButton);
    connect(horizontalKeybindButton, &QPushButton::clicked, this,
            &ConfigWindow::handleHotkeyButton);
    connect(lastOCRKeybindButton, &QPushButton::clicked, this,
            &ConfigWindow::handleHotkeyButton);
}

QMenu *ConfigWindow::createMenu() {
    QAction *quitAction = new QAction("&Quit", this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    QMenu *menu = new QMenu(this);
    menu->addAction(quitAction);

    return menu;
}

void ConfigWindow::closeEvent(QCloseEvent *event) {
#ifdef Q_OS_MACOS
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void ConfigWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        show();
    }
}

void ConfigWindow::handleHotkeyButton() {
    setRegistered(hotkeys, false);
    KeyDialog setKeyDialog;
    auto button = qobject_cast<QPushButton *>(sender());
    if (setKeyDialog.exec() == QDialog::Accepted) {
        button->setText(setKeyDialog.getKeySeq());
        QString key = "Hotkeys/";
        key.append(button->objectName());
        QString value = setKeyDialog.getKeySeq();
        settings->setValue(key, value);
        settings->sync();

        hotkeys[button->objectName().toStdString()]->setShortcut(value);
    }
    setRegistered(hotkeys, true);
}
