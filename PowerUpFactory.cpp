#include "PowerUpFactory.h"

int PowerUpFactory::createRandomPowerUp(RandomGenerator& random) {
    int option = random.randomEntero(1, 4);

    if (option == 1) {
        return POWER_DOUBLE_TURN;
    }
    else if (option == 2) {
        return POWER_MOVE_PRECISION;
    }
    else if (option == 3) {
        return POWER_ATTACK_PRECISION;
    }
    else {
        return POWER_ATTACK_POWER;
    }
}
