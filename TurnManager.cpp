#include "TurnManager.h"

TurnManager::TurnManager() {
	ActualPlayer = 1;
	TurnNumber = 1;
}

int TurnManager::getActualPlayer() {
	return ActualPlayer;
}

int TurnManager::getTurnNumber() {
	return TurnNumber;
}

void TurnManager::nextTurn() {
	if(ActualPlayer==1) {
		ActualPlayer = 2;
	}
	else {
		ActualPlayer = 1;
	}

	TurnNumber++;
}

bool TurnManager::isPlayerTurn(int player) {
	return player == ActualPlayer;
}

