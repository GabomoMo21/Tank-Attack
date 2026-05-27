#include "Game.h"
#include <string>

void Game::finalizarAccion() {
    int jugadorQueTermino = turnManager.getActualPlayer();

    tryGenerateRandomPowerUp(jugadorQueTermino);

    turnManager.nextTurn();
}

void Game::tryGenerateRandomPowerUp(int jugador) {
    int chance = random.randomEntero(1, 100);

    if (chance <= 30) {
        powerUpManager.addRandomPowerUp(jugador, random);
    }
}

void Game::revisarFinDeJuego() {
    if (gameOver) {
        return;
    }

    int player1Alive = tanques.contarVivosJugador(1);
    int player2Alive = tanques.contarVivosJugador(2);

    if (player1Alive == 0) {
        activarVictoria(2);
        return;
    }

    if (player2Alive == 0) {
        activarVictoria(1);
        return;
    }

    if (gameTimer.TimeUp()) {
        int winner = tanques.getWinnerByAliveTanks();

        if (winner == 1) {
            activarVictoria(1);
        }
        else if (winner == 2) {
            activarVictoria(2);
        }
        else {
            activarVictoria(0);
        }

        return;
    }
}

void Game::activarVictoria(int jugadorGanador) {
    if (gameOver) {
        return;
    }

    gameOver = true;
    ganador = jugadorGanador;
    victoryAnimTime = 0.0f;

    tankselected = nullptr;
    hayRuta = false;
    tamanoCamino = 0;
    resetRutaRandomVisual();
}

void Game::actualizarTitulo() {
    int player1Alive = tanques.contarVivosJugador(1);
    int player2Alive = tanques.contarVivosJugador(2);

    std::string titulo =
        "Tank Attack - Player " +
        std::to_string(turnManager.getActualPlayer()) +
        " | Time: " +
        std::to_string(gameTimer.getMinutes()) +
        ":";

    if (gameTimer.getSeconds() < 10) {
        titulo += "0";
    }

    titulo += std::to_string(gameTimer.getSeconds());

    titulo +=
        " | P1 tanks: " +
        std::to_string(player1Alive) +
        " | P2 tanks: " +
        std::to_string(player2Alive);

    window.setTitle(titulo);
}

void Game::resetRutaRandomVisual() {
    rutaRandomVisible = false;

    finIntento1 = -1;
    finRandom = -1;
    finIntento2 = -1;

    vistaInicio1 = -1;
    vistaFin1 = -1;
    vistaInicioRandom = -1;
    vistaFinRandom = -1;
    vistaInicio2 = -1;
    vistaFin2 = -1;
}