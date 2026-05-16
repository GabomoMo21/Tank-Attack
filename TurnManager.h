#pragma once

class TurnManager {
private:
    int actualPlayer;
    int turnNumber;

    int extraTurnPlayer;
    int extraTurnsLeft;

public:
    TurnManager();

    int getActualPlayer();
    int getTurnNumber();

    void nextTurn();
    bool isPlayerTurn(int player);

    void activateDoubleTurn(int player);
};