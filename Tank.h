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
    float visualfila;
	float visualcolumna;
	int targetfila;
	int targetcolumna;
	bool moving;
	float moveSpeed;
    float cannonAngle;

    Tank(int fila, int columna, int tipo, int jugador, sf::Color color);

    int getfila();
    int getcolumna();
    int gettipo();
    int getvida();
    bool estavivo();
	int getplayer();

    sf::Color getcolor();

    float getVisualFila();
    float getVisualColumna();

    void move(int nuevaFila, int nuevaColumna);

    void startMoveTo(int nuevaFila, int nuevaColumna);
    void updateMovement(float deltaTime);
    bool isMoving();

    bool incell(int filaclick, int columnaclick);
    void getdamage(int damage);


};