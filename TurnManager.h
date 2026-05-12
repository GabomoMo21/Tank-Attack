#pragma once

class TurnManager {
private:
	int ActualPlayer, TurnNumber;

public:
	TurnManager();

	int getActualPlayer();
	int getTurnNumber();

	void nextTurn();

	bool isPlayerTurn(int player);

};
