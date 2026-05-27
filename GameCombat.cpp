#include "Game.h"

void Game::dispararTanqueSeleccionado(int targetRow, int targetCol) {
    int player = tankselected->getplayer();

    tankselected->aimAtCell(targetRow, targetCol);

    bool fullPower = powerUpManager.hasAttackPower(player);
    bool attackPrecision = powerUpManager.hasAttackPrecision(player);

    if (attackPrecision) {
        int nodoInicio = grafo.obtenerNodo(
            tankselected->getfila(),
            tankselected->getcolumna()
        );

        int nodoDestino = grafo.obtenerNodo(
            targetRow,
            targetCol
        );

        hayRuta = pathfinder.buscarRutaAStar(
            grafo,
            tanques,
            tankselected,
            nodoInicio,
            nodoDestino,
            camino,
            tamanoCamino
        );

        if (hayRuta && tamanoCamino > 0) {
            bullet.shootAStar(
                tankselected,
                camino,
                tamanoCamino,
                tamanoCelda,
                fullPower,
                grafo
            );

            audioManager.playShoot();

            powerUpManager.useAttackPrecision(player);
            powerUpManager.useAttackPower(player);

            tankselected = nullptr;

            finalizarAccion();
        }
    }
    else {
        bullet.shoot(
            tankselected,
            targetRow,
            targetCol,
            tamanoCelda,
            fullPower
        );

        audioManager.playShoot();

        powerUpManager.useAttackPower(player);

        hayRuta = false;
        tankselected = nullptr;

        finalizarAccion();
    }
}