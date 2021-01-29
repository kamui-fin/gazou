#include <QComboBox>
#include <QHotkey>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QSystemTrayIcon>
#include <vector>

class ConfigWindow : public QMainWindow {
  Q_OBJECT

public:
  ConfigWindow(std::map<std::string, QHotkey *> hotkeys, QWidget *parent = 0);

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  std::map<std::string, QHotkey *> hotkeys;

  QWidget *widget;
  QLabel *verticalKeybindLabel;
  QPushButton *verticalKeybindButton;

  QLabel *horizontalKeybindLabel;
  QPushButton *horizontalKeybindButton;

  QLabel *lastOCRKeybindLabel;
  QPushButton *lastOCRKeybindButton;

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;

  QSettings *settings;

  QMenu *createMenu();
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void handleHotkeyButton();
};