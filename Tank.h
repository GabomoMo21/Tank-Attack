#pragma once

#include <SFML/Graphics.hpp>

class Tank {
public:
    int fila;
    int columna;
    int tipo;
    int jugador;
    int vida;
    bool vivo;
    sf::Color color;

    Tank(int fila, int columna, int tipo, int jugador, sf::Color color);

    int getfila();
    int getcolumna();
    int gettipo();
    int getvida();
    bool estavivo();
	int getplayer();

    sf::Color getcolor();

    void move(int nuevaFila, int nuevaColumna);
    bool incell(int filaclick, int columnaclick);
    void getdamage(int damage);
};