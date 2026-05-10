#pragma once

class RandomGenerator;

class Mapa {
public:
    static const int fil = 15;
    static const int col = 15;

    int m[fil][col];

    Mapa();

    bool recorrible(int valor);
    void generarmaparandom(RandomGenerator& random);
};