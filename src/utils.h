#include <QScreen>
#include <QHotkey>
#include <QClipboard>
#include <map>

QScreen *getActiveScreen();
void remove_spaces(char* s);
void setRegistered(std::map<std::string, QHotkey *> hotkeys, bool registered);
void copyToClipboard(char *text, QClipboard *cb);
