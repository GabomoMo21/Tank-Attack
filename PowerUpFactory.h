#pragma once

#include "PowerUp.h"
#include "RandomGenerator.h"

class PowerUpFactory {
public:
    static int createRandomPowerUp(RandomGenerator& random);
};