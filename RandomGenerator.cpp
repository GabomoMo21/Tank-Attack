#include "RandomGenerator.h"

RandomGenerator::RandomGenerator() : gen(rd()) {
}

int RandomGenerator::randomEntero(int minimo, int maximo) {
    std::uniform_int_distribution<> distribucion(minimo, maximo);
    return distribucion(gen);
}