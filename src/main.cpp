#include <QObject>
#include <QPixmap>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include <QApplication>
#include <QClipboard>
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

int cli(int argc, char **argv) {
    std::vector<std::string> args(argv + 1, argv + argc);
    std::string first = args[0];

    ORIENTATION orn;
    char *result;

    if (first == "--help") {
        help(argv);
        return 0;
    } else if (first == "--prevscan" || first == "-p") {
        result = prevOcr();
        std::cout << result << std::endl;
        return 0;
    } else if (first == "--vertical" || first == "-v") {
        orn = VERTICAL;
    } else if (first == "--horizontal" || first == "-h") {
        orn = HORIZONTAL;
    } else {
        qCritical("Invalid arguments, please use %s --help to "
                  "see the usage",
                  argv[0]);
        return 1;
    }

    if (args.size() > 1) {
        char *img = argv[2];

        if (!pathExist(img)) {
            qCritical("Invalid image path");
            return 1;
        }

        result = ocr->ocrImage(img, orn);
    } else {
        result = interactive(orn);
    }

    std::cout << result << std::endl;
    return 0;
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    ocr = new OCR();
    state.loadLastState(stateFile);

    if (argc > 1) {
        int ret = cli(argc, argv);
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
