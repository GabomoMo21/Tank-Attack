#include "PowerUpQueue.h"

PowerUpNode::PowerUpNode(PowerUp powerUp) {
    this->powerUp = powerUp;
    next = nullptr;
}

PowerUpQueue::PowerUpQueue() {
    front = nullptr;
    end = nullptr;
    count = 0;
}

PowerUpQueue::~PowerUpQueue() {
    while (!empty()) {
        dequeue();
    }
}

bool PowerUpQueue::empty() {
    return front == nullptr;
}

void PowerUpQueue::enqueue(PowerUp powerUp) {
    PowerUpNode* newNode = new PowerUpNode(powerUp);

    if (empty()) {
        front = newNode;
        end = newNode;
    }
    else {
        end->next = newNode;
        end = newNode;
    }

    count++;
}

PowerUp PowerUpQueue::getFront() {
    if (empty()) {
        return PowerUp();
    }

    return front->powerUp;
}

void PowerUpQueue::dequeue() {
    if (empty()) {
        return;
    }

    PowerUpNode* temp = front;
    front = front->next;

    if (front == nullptr) {
        end = nullptr;
    }

    delete temp;
    count--;
}

int PowerUpQueue::getCount() {
    return count;
}

string PowerUpQueue::getText() {
    string text = "";

    PowerUpNode* actual = front;
    int number = 1;

    while (actual != nullptr) {
        text += std::to_string(number);
        text += ": ";
        text += actual->powerUp.getName();
        text += "\n";

        actual = actual->next;
        number++;
    }

    if (text == "") {
        text = "Empty";
    }

    return text;
}