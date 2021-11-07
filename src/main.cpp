#include <QDir>
#include <QObject>
#include <QPixmap>
#include <iostream>
#include <map>
#include <vector>

#include "config.h"
#include "selectorwidget.h"
#include "state.h"
#include "utils.h"

State state;
OCR *ocr;
QClipboard *clipboard;
QString imagePath = getTempImage();

void runRegOCR(ORIENTATION orn) {
    if (!state.getCurrentlySelecting()) {
        state.setCurrentlySelecting(true);
        SelectorWidget sw;
        sw.exec();
        state.setCurrentlySelecting(false);

        char *result = ocr->ocrImage(imagePath, orn);
        clipboard->setText(result);

        LastOCRInfo info = {orn, sw.lastSelectedRect};
        state.setLastOCRInfo(info);

#ifdef DEBUG
        qDebug("%s", result);
#endif
    }
}

void runPrevOCR(ORIENTATION _) {
    QPixmap desktopPixmap = grabScreenshot();
    QPixmap selectedPixmap =
        desktopPixmap.copy(state.getLastOCRInfo().rect.normalized());
    selectedPixmap.toImage().save(imagePath);

    ORIENTATION orn = state.getLastOCRInfo().orn;
    if (orn != NONE) {
        char *result = ocr->ocrImage(imagePath, orn);
        clipboard->setText(result);
#ifdef DEBUG
        qDebug("%s", result);
#endif
    }
}

void help(char **argv) {
    std::cout << "OCR for Japanese texts" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "1) " << argv[0] << std::endl;
    std::cout << "\tRun the main application" << std::endl;
    std::cout << "2) " << argv[0] << " --help" << std::endl;
    std::cout << "\tDisplay this message" << std::endl;
    std::cout << "3) " << argv[0]
              << " ORIENTATION{-h; -v; horizontal; vertical} IMAGEFILE"
              << std::endl;
    std::cout << "\tRun the OCR on the given IMAGEFILE with given "
                 "ORIENTATION."
              << std::endl;
}

int cli(int argc, char **argv) {
    if (!std::strcmp(argv[1], "--help")) {
        help(argv);
        return 0;
    }
    if (argc > 2) {
        char *orientation = argv[1];
        char *img = argv[2];

        ORIENTATION orn;

        if (!pathExist(img)) {
            qCritical("Invalid image path");
            return 1;
        }

        if (!std::strcmp(orientation, "vertical") ||
            !std::strcmp(orientation, "-v")) {
            orn = VERTICAL;
        } else if (!std::strcmp(orientation, "horizontal") ||
                   !std::strcmp(orientation, "-h")) {
            orn = HORIZONTAL;
        } else {
            qCritical("Invalid orientation");
            return 1;
        }

        char *result = ocr->ocrImage(img, orn);
        std::cout << result << std::endl;
    } else {
        qCritical("Invalid arguments, please use %s --help to "
                  "see the usage",
                  argv[0]);
    }
    return 0;
}

int main(int argc, char **argv) {
    ocr = new OCR();

    if (argc > 1) {
        return cli(argc, argv);
    } else {
#ifdef GUI
#include "gui_x11.h"
        return startGui(argc, argv, clipboard, runRegOCR, runPrevOCR);
#else
        qCritical("No GUI support was built. Re-compile with -DGUI=ON");
        return 1;
#endif
    }
}
