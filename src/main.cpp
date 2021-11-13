#include <QDir>
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
#include <QHotkey>
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
    QPixmap desktopPixmap = grabScreenshot();
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
    std::cout << "OCR for Japanese texts" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "1) " << argv[0] << std::endl;
    std::cout << "\tRun the main application" << std::endl;
    std::cout << "2) " << argv[0] << " --help" << std::endl;
    std::cout << "\tDisplay this message" << std::endl;
    std::cout << "3) " << argv[0] << " prevscan" << std::endl;
    std::cout << "\tRun the OCR on the same coordinates of the previous scan"
              << std::endl;
    std::cout << "4) " << argv[0]
              << " ORIENTATION{-h; -v; horizontal; vertical}" << std::endl;
    std::cout << "\tInteractively run the OCR and print the output to stdout"
              << std::endl;
    std::cout << "5) " << argv[0]
              << " ORIENTATION{-h; -v; horizontal; vertical} IMAGEFILE"
              << std::endl;
    std::cout << "\tRun the OCR on the given IMAGEFILE with the given "
                 "ORIENTATION."
              << std::endl;
}

int cli(int argc, char **argv) {
    std::vector<std::string> args(argv + 1, argv + argc);
    std::string first = args[0];

    ORIENTATION orn;
    char *result;

    if (first == "--help") {
        help(argv);
        return 0;
    } else if (first == "prevscan") {
        result = prevOcr();
        std::cout << result << std::endl;
        return 0;
    } else if (first == "vertical" || first == "-v") {
        orn = VERTICAL;
    } else if (first == "horizontal" || first == "-h") {
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
#include "gui_x11.h"
        startGui(&app, interactive, prevOcr);
        return app.exec();
#else
        qCritical("No GUI support was built. Re-compile with -DGUI=ON or use "
                  "the CLI (run with --help to view the available options)");
        return 1;
#endif
    }
}
