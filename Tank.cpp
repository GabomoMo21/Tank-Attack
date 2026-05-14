#include "Tank.h"

Tank::Tank(int fila, int columna, int tipo, int jugador, sf::Color color) {
    this->fila = fila;
    this->columna = columna;
    this->tipo = tipo;
    this->jugador = jugador;
    this->vida = 100;
    this->vivo = true;
    this->color = color;
}

int Tank::getfila() {
    return fila;
}

int Tank::getcolumna() {
    return columna;
}

int Tank::gettipo() {
    return tipo;
}

int Tank::getvida() {
    return vida;
}

bool Tank::estavivo() {
    return vivo;
}

sf::Color Tank::getcolor() {
    return color;
}

void Tank::move(int nuevaFila, int nuevaColumna) {
    fila = nuevaFila;
    columna = nuevaColumna;
}

bool Tank::incell(int filaclick, int columnaclick) {
    return fila == filaclick && columna == columnaclick;
}

void Tank::getdamage(int damage) {
    vida -= damage;

    if (vida <= 0) {
        vida = 0;
        vivo = false;
    }
}

int Tank::getplayer() {
    return jugador;
}

