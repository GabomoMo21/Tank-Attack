#pragma once
#include <string>

using namespace std;

const int POWER_NONE = 0;
const int POWER_DOUBLE_TURN = 1;
const int POWER_MOVE_PRECISION = 2;
const int POWER_ATTACK_PRECISION = 3;
const int POWER_ATTACK_POWER = 4;

class PowerUp {
private:
    int type;

public:
    PowerUp();
    PowerUp(int type);

    int getType();
    string getName();
};