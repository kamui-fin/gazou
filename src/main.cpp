#include <QObject>
#include <QPixmap>
#include <fstream>
#include <iostream>
#include <map>
#include <unistd.h>
#include <vector>

#include <QApplication>
#include <QClipboard>
#include <QCommandLineParser>
#include <QDir>
#include <QObject>
#include <QPixmap>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QThread>
#include <QTimer>
#include <map>
#include <qchar.h>
#include <qclipboard.h>
#include <qdir.h>
#include <qstandardpaths.h>
#include <string>
#include <vector>

#include "config.h"
#include "ocr.h"
#include "selectorwidget.h"
#include "state.h"
#include "utils.h"

#ifdef GUI
#include "gui.h"
#endif

State state;
OCR *ocr;
QString imagePath = getTempImage();
std::string stateFile = getCoordsFile();

char *interactive(ORIENTATION orn) {
    char *result = "";
    if (!state.getCurrentlySelecting()) {
        state.setCurrentlySelecting(true);
        SelectorWidget sw;
        sw.exec();
        state.setCurrentlySelecting(false);

        result = ocr->ocrImage(imagePath, orn);
        LastOCRInfo info = {orn, sw.lastSelectedRect};
        state.setLastOCRInfo(info);
        state.saveLastState(stateFile);
#ifdef DEBUG
        qDebug("%s", result);
#endif
    }
    return result;
}

char *prevOcr(ORIENTATION _ = ORIENTATION::NONE) {
    bool ok = true;
    QPixmap desktopPixmap = grabScreenshot(ok);
    if (!ok) {
        qCritical("Unable to screenshot");
        exit(-1);
    }
    QPixmap selectedPixmap =
        desktopPixmap.copy(state.getLastOCRInfo().rect.normalized());
    selectedPixmap.toImage().save(imagePath);
    char *result = "";
    ORIENTATION orn = state.getLastOCRInfo().orn;
    if (orn != NONE) {
        result = ocr->ocrImage(imagePath, orn);
#ifdef DEBUG
        qDebug("%s", result);
#endif
    }
    return result;
}

void help(char **argv) {
    // toggle text processing
    std::cout << "Usage : gazou [OPTIONS] [IMAGEFILE]" << std::endl;
#ifdef GUI
    std::cout << "Will launch GUI if no options are given." << std::endl;
#else
    std::cout << "Built without a GUI." << std::endl;
#endif
    std::cout << "" << std::endl;
    std::cout << "  --help            Display this help menu." << std::endl;
    std::cout << "  -p, --prevscan    Run the OCR on the same coordinates of "
                 "the previous scan."
              << std::endl;
    std::cout << "  -h, --horizontal  Run horizontal OCR." << std::endl;
    std::cout << "  -v, --vertical    Run vertical OCR." << std::endl;
}

int cli(QApplication *app) {

    QCommandLineParser parser;
#ifdef GUI
    parser.setApplicationDescription(
        "Launches GUI if no options are provided.");
#else
    parser.setApplicationDescription("Built without a GUI");
#endif

    parser.addOptions(
        {{{"p", "prevscan"},
          QCoreApplication::translate(
              "main",
              "Run the OCR on the same coordinates of the previous scan")},
         {
             {"l", "language"},
             QCoreApplication::translate(
                 "main", "Specify OCR language, defaults to jpn. "
                         "Options: jpn, chi_sim, chi_trad"),
             QCoreApplication::translate("main", "language"),
         },
         {
             {"v", "vertical"},
             QCoreApplication::translate(
                 "main", "Switch orientation to vertical. Without this, gazou "
                         "expects horizontal text."),
         },
         {
             "version",
             QCoreApplication::translate(
                 "main", "Fetch the version information of gazou"),
         },
         {"help", QCoreApplication::translate("main", "View this help menu")}});

    parser.addPositionalArgument(
        "imagePath",
        QCoreApplication::translate("main", "Source image file to OCR"));

    parser.process(*app);

    if (parser.isSet("help")) {
        parser.showHelp();
    } else if (parser.isSet("version")) {
        parser.showVersion();
    }

    if (parser.isSet("language")) {
        QString lang = parser.value("l");
        if (!(lang == "jpn" || lang == "chi_sim" || lang == "chi_trad")) {
            qCritical("Invalid language");
            return 1;
        }
        QSettings *settings = new QSettings("gazou", "gazou");
        settings->setValue("language", lang);
        settings->sync();
    }

    if (parser.isSet("prevscan")) {
        std::cout << prevOcr() << std::endl;
        return 0;
    }

    ORIENTATION orn = HORIZONTAL;
    if (parser.isSet("vertical")) {
        orn = VERTICAL;
    }

    const QStringList posArgs = parser.positionalArguments();
    if (!isatty(fileno(stdin))) {
        std::cout << ocr->ocrImage("", orn, true) << std::endl;
    } else if (posArgs.isEmpty()) {
        std::cout << interactive(orn) << std::endl;
    } else {
        QString imagePath = posArgs.at(0);
        if (!pathExist(imagePath.toStdString().c_str())) {
            qCritical("Invalid image path");
            return 1;
        }
        std::cout << ocr->ocrImage(imagePath, orn) << std::endl;
    }

    return 0;
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("gazou");
    QApplication::setApplicationVersion("0.3.0");

    ocr = new OCR();
    state.loadLastState(stateFile);

    if (argc > 1 || !isatty(fileno(stdin))) {
        int ret = cli(&app);
        return ret;
    } else {
#ifdef GUI
        startGui(&app, interactive, prevOcr);
        return app.exec();
#else
        qCritical("No GUI support was built. Re-compile with -DGUI=ON or use "
                  "the CLI (run with --help to view the available options)");
        return 1;
#endif
    }
}
