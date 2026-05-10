#include "TankFactory.h"

Tank TankFactory::creartanqueazul(int fila, int columna) {
    return Tank(fila, columna, 1, 1, sf::Color::Blue);
}

Tank TankFactory::crearTanqueRojo(int fila, int columna) {
    return Tank(fila, columna, 2, 1, sf::Color::Red);
}

Tank TankFactory::crearTanqueCeleste(int fila, int columna) {
    return Tank(fila, columna, 1, 2, sf::Color::Cyan);
}

Tank TankFactory::crearTanqueAmarillo(int fila, int columna) {
    return Tank(fila, columna, 2, 2, sf::Color::Yellow);
}