#include <QClipboard>
#include <QScreen>
#include <map>

QScreen *getActiveScreen();
void remove_spaces(char *s);
QPixmap grabScreenshot();
bool pathExist(const char *s);
QString getTempImage(bool debugFile = false);
std::string getCoordsFile();
const char *convertToCString(QString src);
std::vector<std::string> split(std::string s, std::string delimiter);
