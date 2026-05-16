#include "TurnManager.h"

TurnManager::TurnManager() {
	actualPlayer = 1;
	turnNumber = 1;
	extraTurnPlayer = 0;
	extraTurnsLeft = 0;
}

int TurnManager::getActualPlayer() {
	return actualPlayer;
}

int TurnManager::getTurnNumber() {
	return turnNumber;
}

void TurnManager::nextTurn() {
    turnNumber++;

    if (extraTurnsLeft > 0 && actualPlayer == extraTurnPlayer) {
        extraTurnsLeft--;
        return;
    }

    if (actualPlayer == 1) {
        actualPlayer = 2;
    }
    else {
        actualPlayer = 1;
    }
}
void TurnManager::activateDoubleTurn(int player) {
    extraTurnPlayer = player;
    extraTurnsLeft = 2;
}

bool TurnManager::isPlayerTurn(int player) {
	return player == actualPlayer;
}

