#include "PowerUp.h"

PowerUp::PowerUp() {
    type = POWER_NONE;
}

PowerUp::PowerUp(int type) {
    this->type = type;
}

int PowerUp::getType() {
    return type;
}

string PowerUp::getName() {
    if (type == POWER_DOUBLE_TURN) {
        return "DT";
    }

    if (type == POWER_MOVE_PRECISION) {
        return "MP";
    }

    if (type == POWER_ATTACK_PRECISION) {
        return "AP";
    }

    if (type == POWER_ATTACK_POWER) {
        return "PW";
    }

    return "--";
}