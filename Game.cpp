#include "Game.h"

Game::Game()
    : gameTimer(300.0f),
    window(sf::VideoMode({ 800u, 660u }), "Tank Attack", sf::Style::Resize),
    tankselected(nullptr),
    tamanoCamino(0),
    hayRuta(false),
    movimientoEnProgreso(false),
    movingTank(nullptr),
    tamanoCaminoAnimado(0),
    indiceCaminoAnimado(0),
    jugadorMovimientoPendiente(0),
    gameOver(false),
    ganador(0),
    victoryAnimTime(0.0f),
    tamanoCelda(40.0f),
    headerAltura(60.0f),
    mapaOffsetX(0.0f),
    mapaOffsetY(60.0f),
    finIntento1(-1),
    finRandom(-1),
    finIntento2(-1),
    vistaInicio1(-1),
    vistaFin1(-1),
    vistaInicioRandom(-1),
    vistaFinRandom(-1),
    vistaInicio2(-1),
    vistaFin2(-1),
    rutaRandomVisible(false)
{
}

int Game::run() {
    inicializarTanques();
    generarMapaValido();

    if (!cargarAssets()) {
        return -1;
    }

    while (window.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();

        revisarFinDeJuego();
        procesarEventos();
        actualizar(deltaTime);
        actualizarTitulo();
        dibujar();
    }

    return 0;
}

void Game::actualizar(float deltaTime) {
    if (gameOver) {
        victoryAnimTime += deltaTime;
        return;
    }

    actualizarMovimientoAnimado(deltaTime);

    bullet.update(deltaTime, map, tanques, tamanoCelda);
}