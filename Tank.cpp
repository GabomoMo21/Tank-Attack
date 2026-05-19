#include "Tank.h"
#include <cmath>

Tank::Tank(int fila, int columna, int tipo, int jugador, sf::Color color) {
    this->fila = fila;
    this->columna = columna;
    this->tipo = tipo;
    this->jugador = jugador;
    this->vida = 100;
    this->vivo = true;
    this->color = color;

    this->visualfila = float(fila);
    this->visualcolumna = float(columna);

    this->targetfila = fila;
    this->targetcolumna = columna;

    this->moving = false;
    this->moveSpeed = 6.0f;

    this->cannonAngle = -90.0f;
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

float Tank::getVisualFila() {
	return visualfila;
}

float Tank::getVisualColumna() {
	return visualcolumna;
}

void Tank::startMoveTo(int nuevaFila, int nuevaColumna) {
	targetfila = nuevaFila;
	targetcolumna = nuevaColumna;
	moving = true;
}

bool Tank::isMoving() {
	return moving;
}

void Tank::updateMovement(float deltaTime) {
    if (!moving) {
        return;
    }

    float df = float(targetfila) - visualfila;
    float dc = float(targetcolumna) - visualcolumna;

    float distancia = std::sqrt(df * df + dc * dc);

    if (distancia <= 0.01f) {
        targetfila = float(targetfila);
        visualcolumna = float(targetcolumna);

        fila = targetfila;
        columna = targetcolumna;

        moving = false;
        return;
    }

    float paso = moveSpeed * deltaTime;

    if (paso >= distancia) {
        visualfila = float(targetfila);
        visualcolumna = float(targetcolumna);

        fila = targetfila;
        columna = targetcolumna;

        moving = false;
    }
    else {
        visualfila += (df / distancia) * paso;
        visualcolumna += (dc / distancia) * paso;
    }
}


