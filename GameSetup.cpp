#include "Game.h"
#include <iostream>

void Game::inicializarTanques() {
    tanques.agregar(TankFactory::creartanqueazul(1, 1));
    tanques.agregar(TankFactory::creartanqueazul(1, 2));
    tanques.agregar(TankFactory::crearTanqueRojo(2, 1));
    tanques.agregar(TankFactory::crearTanqueRojo(2, 2));

    tanques.agregar(TankFactory::crearTanqueCeleste(13, 13));
    tanques.agregar(TankFactory::crearTanqueCeleste(13, 12));
    tanques.agregar(TankFactory::crearTanqueAmarillo(12, 13));
    tanques.agregar(TankFactory::crearTanqueAmarillo(12, 12));
}

bool Game::cargarAssets() {
    if (!suelo.loadFromFile("suelo.png")) {
        std::cout << "Error loading suelo.png\n";
        return false;
    }

    if (!pared.loadFromFile("pared.png")) {
        std::cout << "Error loading pared.png\n";
        return false;
    }

    if (!font.openFromFile("PixelifySans-VariableFont_wght.ttf")) {
        std::cout << "Error loading font\n";
        return false;
    }

    audioManager.load();
    audioManager.playMusic();

    return true;
}

void Game::generarMapaValido() {
    do {
        map.generarmaparandom(random);
    } while (!pathfinder.validacionBFS(map));

    grafo.generarMatrizadyacencia(map);
}