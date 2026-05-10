#include "Mapa.h"
#include "RandomGenerator.h"

Mapa::Mapa() {
    for (int i = 0; i < fil; i++) {
        for (int j = 0; j < col; j++) {
            m[i][j] = 0;
        }
    }
}

bool Mapa::recorrible(int valor) {
    return valor == 0;
}

void Mapa::generarmaparandom(RandomGenerator& random) {
    for (int i = 0; i < fil; i++) {
        for (int j = 0; j < col; j++) {
            if (i == 0 || i == fil - 1 || j == 0 || j == col - 1) {
                m[i][j] = 1;
            }
            else {
                m[i][j] = 0;
            }
        }
    }

    for (int i = 1; i < fil - 1; i++) {
        for (int j = 1; j < col - 1; j++) {
            bool isSpawnCell =
                (i == 1 && j == 1) ||
                (i == 1 && j == 2) ||
                (i == 2 && j == 1) ||
                (i == 2 && j == 2) ||
                (i == fil - 2 && j == col - 2) ||
                (i == fil - 2 && j == col - 3) ||
                (i == fil - 3 && j == col - 2) ||
                (i == fil - 3 && j == col - 3);

            if (isSpawnCell) {
                continue;
            }

            if (random.randomEntero(1, 100) <= 25) {
                m[i][j] = 1;
            }
        }
    }
}