#pragma once

#include "PowerUpQueue.h"
#include "RandomGenerator.h"
#include "TurnManager.h"

class PowerUpManager {
private:
    PowerUpQueue player1Queue;
    PowerUpQueue player2Queue;

    bool movePrecisionPlayer1;
    bool movePrecisionPlayer2;

    bool attackPrecisionPlayer1;
    bool attackPrecisionPlayer2;

    bool attackPowerPlayer1;
    bool attackPowerPlayer2;

public:
    PowerUpManager();

    void addRandomPowerUp(int player, RandomGenerator& random);
    void addPowerUp(int player, int type);

    void consumePowerUp(int player, TurnManager& turnManager);

    bool hasMovePrecision(int player);
    bool hasAttackPrecision(int player);
    bool hasAttackPower(int player);

    void useMovePrecision(int player);
    void useAttackPrecision(int player);
    void useAttackPower(int player);

    string getQueueText(int player);
};