#pragma once

#include <random>

class RandomGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;

public:
    RandomGenerator();

    int randomEntero(int minimo, int maximo);
};