#pragma once

#include <SFML/Graphics.hpp>
#include "Tank.h"

class NodoTank {
public:
    Tank tank;
    NodoTank* next;

    NodoTank(Tank tank);
};

class listaTank {
private:
    NodoTank* head;

public:
    listaTank();
    ~listaTank();

    void agregar(Tank tank);

    Tank* buscarTanqueEnCelda(int fila, int columna);

    bool celdaOcupada(int fila, int columna);

    int contarVivosJugador(int jugador);

    int getWinnerByAliveTanks();

    void dibujarTodos(sf::RenderWindow& ventana,
        float tamanoCelda,
        float offsetX,
        float offsetY);

    bool cellOccupiedExcept(int fila, int columna, Tank* tanqueIgnorado);

    void drawlife(
        sf::RenderWindow& ventana,
        sf::Font& font,
        int jugador,
        float x,
        float y
    );

};