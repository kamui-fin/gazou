#include "state.h"

State::State() {
    this->currentlySelecting = false;
    this->lastInfo.orn = NONE;
}

bool State::getCurrentlySelecting() { return this->currentlySelecting; }

void State::setCurrentlySelecting(bool curSel) {
    this->currentlySelecting = curSel;
}

LastOCRInfo State::getLastOCRInfo() { return lastInfo; };

void State::setLastOCRInfo(LastOCRInfo inf) { this->lastInfo = inf; };
