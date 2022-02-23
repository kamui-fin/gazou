#include <QClipboard>
#include <QScreen>
#include <map>

#define UTF8_CHAR_LEN(byte) ((0xE5000000 >> ((byte >> 3) & 0x1e)) & 3) + 1

QScreen *getActiveScreen();
void remove_spaces(char *s);
QPixmap grabScreenshot(bool &ok);
bool pathExist(const char *s);
QString getTempImage(bool debugFile = false);
std::string getCoordsFile();
const char *convertToCString(QString src);
std::vector<std::string> split(std::string s, std::string delimiter);
int get_char(char *input, int offset, char *character);

// lang utils
bool isJapanese(char *text);
bool isKanji(char *text);
bool isHiragana(char *text);
bool isKatakana(char *text);
