#pragma once

#include "Tank.h"

class TankFactory {
public:
    static Tank creartanqueazul(int fila, int columna);
    static Tank crearTanqueRojo(int fila, int columna);
    static Tank crearTanqueCeleste(int fila, int columna);
    static Tank crearTanqueAmarillo(int fila, int columna);
};