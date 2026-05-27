#include "Game.h"

void Game::moverTanqueSeleccionado(int filaDestino, int columnaDestino) {
    if (
        !map.recorrible(map.m[filaDestino][columnaDestino]) ||
        tanques.celdaOcupada(filaDestino, columnaDestino)
        ) {
        return;
    }

    int nodoInicio = grafo.obtenerNodo(
        tankselected->getfila(),
        tankselected->getcolumna()
    );

    int nodoDestino = grafo.obtenerNodo(
        filaDestino,
        columnaDestino
    );

    int chance = random.randomEntero(1, 100);
    int player = tankselected->getplayer();

    int typeOneLimit = 50;
    int typeTwoLimit = 80;

    if (powerUpManager.hasMovePrecision(player)) {
        typeOneLimit = 90;
        typeTwoLimit = 90;
    }

    if (tankselected->gettipo() == 1) {
        if (chance <= typeOneLimit) {
            resetRutaRandomVisual();

            hayRuta = pathfinder.buscarRutaBFS(
                grafo,
                tanques,
                tankselected,
                nodoInicio,
                nodoDestino,
                camino,
                tamanoCamino
            );
        }
        else {
            calcularRutaRandom(nodoInicio, nodoDestino);
        }
    }
    else {
        if (chance <= typeTwoLimit) {
            resetRutaRandomVisual();

            hayRuta = pathfinder.buscarRutaDijkstra(
                grafo,
                tanques,
                tankselected,
                nodoInicio,
                nodoDestino,
                camino,
                tamanoCamino
            );
        }
        else {
            calcularRutaRandom(nodoInicio, nodoDestino);
        }
    }

    if (hayRuta && tamanoCamino > 0) {
        iniciarMovimientoAnimado(
            tankselected,
            camino,
            tamanoCamino,
            player
        );
    }

    tankselected = nullptr;
}

void Game::calcularRutaRandom(int nodoInicio, int nodoDestino) {
    rutaRandomVisible = true;

    vistaInicio1 = nodoInicio;
    vistaFin1 = nodoDestino;
    vistaInicioRandom = -1;
    vistaFinRandom = -1;
    vistaInicio2 = -1;
    vistaFin2 = -1;

    hayRuta = pathfinder.rutalineavistrandom(
        grafo,
        map,
        tanques,
        tankselected,
        nodoInicio,
        nodoDestino,
        camino,
        tamanoCamino,
        finIntento1,
        finRandom,
        finIntento2,
        4,
        random
    );

    if (tamanoCamino > 1) {
        vistaInicio2 = camino[tamanoCamino - 1];
        vistaFin2 = nodoDestino;
    }
}

void Game::iniciarMovimientoAnimado(
    Tank* tank,
    int camino[],
    int tamano,
    int jugador
) {
    if (tank == nullptr || tamano <= 1) {
        return;
    }

    movingTank = tank;
    tamanoCaminoAnimado = tamano;
    indiceCaminoAnimado = 1;
    movimientoEnProgreso = true;
    jugadorMovimientoPendiente = jugador;

    for (int i = 0; i < tamano; i++) {
        caminoAnimado[i] = camino[i];
    }

    int siguienteNodo = caminoAnimado[indiceCaminoAnimado];

    int siguienteFila = grafo.obtenerFila(siguienteNodo);
    int siguienteColumna = grafo.obtenerColumna(siguienteNodo);

    movingTank->startMoveTo(siguienteFila, siguienteColumna);
    audioManager.playMove();
}

void Game::actualizarMovimientoAnimado(float deltaTime) {
    if (!movimientoEnProgreso || movingTank == nullptr) {
        return;
    }

    movingTank->updateMovement(deltaTime);

    if (!movingTank->isMoving()) {
        indiceCaminoAnimado++;

        if (indiceCaminoAnimado < tamanoCaminoAnimado) {
            int siguienteNodo = caminoAnimado[indiceCaminoAnimado];

            int siguienteFila = grafo.obtenerFila(siguienteNodo);
            int siguienteColumna = grafo.obtenerColumna(siguienteNodo);

            movingTank->startMoveTo(siguienteFila, siguienteColumna);
        }
        else {
            audioManager.stopMove();

            movimientoEnProgreso = false;

            powerUpManager.useMovePrecision(jugadorMovimientoPendiente);

            finalizarAccion();

            movingTank = nullptr;
            jugadorMovimientoPendiente = 0;

            hayRuta = false;
            tamanoCamino = 0;

            resetRutaRandomVisual();
        }
    }
}