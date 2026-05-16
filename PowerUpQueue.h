#pragma once
#include "PowerUp.h"

class PowerUpNode {
public:
    PowerUp powerUp;
    PowerUpNode* next;

    PowerUpNode(PowerUp powerUp);
};

class PowerUpQueue {
private:
    PowerUpNode* front;
    PowerUpNode* end;
    int count;

public:
    PowerUpQueue();
    ~PowerUpQueue();

    bool empty();
    void enqueue(PowerUp powerUp);
    PowerUp getFront();
    void dequeue();

    int getCount();
    string getText();
};