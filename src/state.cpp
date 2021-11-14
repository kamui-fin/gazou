#include "state.h"
#include "utils.h"
#include <fstream>
#include <iostream>

State::State() {
    this->currentlySelecting = false;
    this->lastInfo.orn = NONE;
}

bool State::getCurrentlySelecting() { return this->currentlySelecting; }
void State::setCurrentlySelecting(bool curSel) {
    this->currentlySelecting = curSel;
}

LastOCRInfo State::getLastOCRInfo() { return lastInfo; }
void State::setLastOCRInfo(LastOCRInfo inf) { this->lastInfo = inf; }

void State::saveLastState(std::string outFile) {
    QRect rect = this->lastInfo.rect;
    std::ofstream file(outFile);
    file << this->lastInfo.orn << std::endl;
    file << rect.x() << "," << rect.y() << "," << rect.width() << ","
         << rect.height() << std::endl;
    file.close();
}

void State::loadLastState(std::string stateFile) {
    std::ifstream file(stateFile);
    if (!file.is_open()) {
        return;
    }

    ORIENTATION orn;
    QRect rect;

    std::string tempOrn;
    std::getline(file, tempOrn);
    orn = static_cast<ORIENTATION>(std::stoi(tempOrn));

    std::string tempRect;
    std::string delim = ",";
    std::getline(file, tempRect);
    std::vector<std::string> splitted = split(tempRect, delim);
    rect = QRect(stoi(splitted[0]), stoi(splitted[1]), stoi(splitted[2]),
                 stoi(splitted[3]));

    LastOCRInfo info = {orn, rect};
    this->setLastOCRInfo(info);
}
