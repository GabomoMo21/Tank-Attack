#include "PowerUpManager.h"
#include "PowerUp.h"

PowerUpManager::PowerUpManager() {
    movePrecisionPlayer1 = false;
    movePrecisionPlayer2 = false;

    attackPrecisionPlayer1 = false;
    attackPrecisionPlayer2 = false;

    attackPowerPlayer1 = false;
    attackPowerPlayer2 = false;
}

void PowerUpManager::addRandomPowerUp(int player, RandomGenerator& random) {
    int type = random.randomEntero(1, 4);
    addPowerUp(player, type);
}

void PowerUpManager::addPowerUp(int player, int type) {
    PowerUp powerUp(type);

    if (player == 1) {
        player1Queue.enqueue(powerUp);
    }
    else {
        player2Queue.enqueue(powerUp);
    }
}

void PowerUpManager::consumePowerUp(int player, TurnManager& turnManager) {
    PowerUp powerUp;

    if (player == 1) {
        if (player1Queue.empty()) {
            return;
        }

        powerUp = player1Queue.getFront();
        player1Queue.dequeue();
    }
    else {
        if (player2Queue.empty()) {
            return;
        }

        powerUp = player2Queue.getFront();
        player2Queue.dequeue();
    }

    int type = powerUp.getType();

    if (type == POWER_DOUBLE_TURN) {
        turnManager.activateDoubleTurn(player);
    }

    if (type == POWER_MOVE_PRECISION) {
        if (player == 1) {
            movePrecisionPlayer1 = true;
        }
        else {
            movePrecisionPlayer2 = true;
        }
    }

    if (type == POWER_ATTACK_PRECISION) {
        if (player == 1) {
            attackPrecisionPlayer1 = true;
        }
        else {
            attackPrecisionPlayer2 = true;
        }
    }

    if (type == POWER_ATTACK_POWER) {
        if (player == 1) {
            attackPowerPlayer1 = true;
        }
        else {
            attackPowerPlayer2 = true;
        }
    }

    turnManager.nextTurn();
}

bool PowerUpManager::hasMovePrecision(int player) {
    if (player == 1) {
        return movePrecisionPlayer1;
    }

    return movePrecisionPlayer2;
}

bool PowerUpManager::hasAttackPrecision(int player) {
    if (player == 1) {
        return attackPrecisionPlayer1;
    }

    return attackPrecisionPlayer2;
}

bool PowerUpManager::hasAttackPower(int player) {
    if (player == 1) {
        return attackPowerPlayer1;
    }

    return attackPowerPlayer2;
}

void PowerUpManager::useMovePrecision(int player) {
    if (player == 1) {
        movePrecisionPlayer1 = false;
    }
    else {
        movePrecisionPlayer2 = false;
    }
}

void PowerUpManager::useAttackPrecision(int player) {
    if (player == 1) {
        attackPrecisionPlayer1 = false;
    }
    else {
        attackPrecisionPlayer2 = false;
    }
}

void PowerUpManager::useAttackPower(int player) {
    if (player == 1) {
        attackPowerPlayer1 = false;
    }
    else {
        attackPowerPlayer2 = false;
    }
}

string PowerUpManager::getQueueText(int player) {
    if (player == 1) {
        return player1Queue.getText();
    }

    return player2Queue.getText();
}