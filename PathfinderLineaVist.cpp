#include "Pathfinder.h"
#include "TankList.h"
#include "Tank.h"
#include <cstdlib>

bool Pathfinder::rutalineavist(
    Grafo& grafo,
    Mapa& map,
    listaTank& tanques,
    Tank* tanqueActual,
    int nodoInicio,
    int nodoDestino,
    int camino[],
    int& tamanoCamino
) {
    tamanoCamino = 0;

    if (nodoInicio == -1 || nodoDestino == -1) {
        return false;
    }

    int filaDestino = grafo.obtenerFila(nodoDestino);
    int columnaDestino = grafo.obtenerColumna(nodoDestino);

    int filaActual = grafo.obtenerFila(nodoInicio);
    int columnaActual = grafo.obtenerColumna(nodoInicio);

    camino[tamanoCamino] = nodoInicio;
    tamanoCamino++;

    while (filaActual != filaDestino || columnaActual != columnaDestino) {
        int diferenciaFila = filaDestino - filaActual;
        int diferenciaColumna = columnaDestino - columnaActual;

        int siguienteFila = filaActual;
        int siguienteColumna = columnaActual;

        // This moves only one axis per step
        if (abs(diferenciaFila) >= abs(diferenciaColumna)) {
            if (diferenciaFila > 0) {
                siguienteFila++;
            }
            else if (diferenciaFila < 0) {
                siguienteFila--;
            }
        }
        else {
            if (diferenciaColumna > 0) {
                siguienteColumna++;
            }
            else if (diferenciaColumna < 0) {
                siguienteColumna--;
            }
        }

        if (
            siguienteFila < 0 ||
            siguienteFila >= Mapa::fil ||
            siguienteColumna < 0 ||
            siguienteColumna >= Mapa::col
            ) {
            break;
        }

        // This stops before a wall
        if (!map.recorrible(map.m[siguienteFila][siguienteColumna])) {
            break;
        }

        // This stops before another tank
        if (tanques.cellOccupiedExcept(siguienteFila, siguienteColumna, tanqueActual)) {
            break;
        }

        filaActual = siguienteFila;
        columnaActual = siguienteColumna;

        camino[tamanoCamino] = grafo.obtenerNodo(filaActual, columnaActual);
        tamanoCamino++;
    }

    // It returns true only if it reached the target
    return camino[tamanoCamino - 1] == nodoDestino;
}
bool Pathfinder::rutalineavistrandom(
    Grafo& grafo,
    Mapa& map,
    listaTank& tanques,
    Tank* tanqueActual,
    int nodoInicio,
    int nodoDestino,
    int camino[],
    int& tamanoCamino,
    int& finIntento1,
    int& finRandom,
    int& finIntento2,
    int radio,
    RandomGenerator& random
) {
    tamanoCamino = 0;
    finIntento1 = -1;
    finRandom = -1;
    finIntento2 = -1;
    int caminoPrimerIntento[Grafo::totalNodos];
    int tamanoPrimerIntento = 0;

    // First try: line view to the original target
    bool llegoPrimero = rutalineavist(
        grafo,
        map,
        tanques,
        tanqueActual,
        nodoInicio,
        nodoDestino,
        caminoPrimerIntento,
        tamanoPrimerIntento
    );

    for (int i = 0; i < tamanoPrimerIntento; i++) {
        camino[tamanoCamino] = caminoPrimerIntento[i];
        tamanoCamino++;
    }

    finIntento1 = tamanoCamino - 1;

    if (llegoPrimero) {
        finRandom = finIntento1;
        finIntento2 = finIntento1;
        return true;
    }

    if (tamanoCamino <= 1) {
        return false;
    }

    // The random correction starts from the last reached cell
    int ultimoNodo = camino[tamanoCamino - 1];

    int filaBase = grafo.obtenerFila(ultimoNodo);
    int columnaBase = grafo.obtenerColumna(ultimoNodo);

    int filaDestino = grafo.obtenerFila(nodoDestino);
    int columnaDestino = grafo.obtenerColumna(nodoDestino);

    int distanciaBase =
        abs(filaDestino - filaBase) +
        abs(columnaDestino - columnaBase);

    int filaRandom = -1;
    int columnaRandom = -1;
    bool encontroRandom = false;

    // This searches one useful random target.
    // It cannot be behind the tank.
    for (int intento = 0; intento < 30; intento++) {
        int posibleFila = filaBase + random.randomEntero(-radio, radio);
        int posibleColumna = columnaBase + random.randomEntero(-radio, radio);

        if (
            posibleFila < 0 ||
            posibleFila >= Mapa::fil ||
            posibleColumna < 0 ||
            posibleColumna >= Mapa::col
            ) {
            continue;
        }

        if (!map.recorrible(map.m[posibleFila][posibleColumna])) {
            continue;
        }

        if (tanques.cellOccupiedExcept(posibleFila, posibleColumna, tanqueActual)) {
            continue;
        }

        int nodoRandomTemporal = grafo.obtenerNodo(posibleFila, posibleColumna);

        if (nodoRandomTemporal == ultimoNodo) {
            continue;
        }

        int distanciaRandom =
            abs(filaDestino - posibleFila) +
            abs(columnaDestino - posibleColumna);

        int direccionDestinoFila = filaDestino - filaBase;
        int direccionDestinoColumna = columnaDestino - columnaBase;

        int direccionRandomFila = posibleFila - filaBase;
        int direccionRandomColumna = posibleColumna - columnaBase;

        int productoDireccion =
            direccionDestinoFila * direccionRandomFila +
            direccionDestinoColumna * direccionRandomColumna;

        // This avoids going backwards.
        // It can go forward or sideways.
        bool noVaHaciaAtras = productoDireccion >= 0;

        // This avoids choosing a random cell much worse than the current cell.
        bool noSeAlejaMucho = distanciaRandom <= distanciaBase + 1;

        if (noVaHaciaAtras && noSeAlejaMucho) {
            filaRandom = posibleFila;
            columnaRandom = posibleColumna;
            encontroRandom = true;
            break;
        }
    }

    if (!encontroRandom) {
        finRandom = finIntento1;
        finIntento2 = finIntento1;
        return tamanoCamino > 1;
    }

    int nodoRandom = grafo.obtenerNodo(filaRandom, columnaRandom);

    int caminoRandom[Grafo::totalNodos];
    int tamanoRandom = 0;

    // Random correction: line view to the random cell
    rutalineavist(
        grafo,
        map,
        tanques,
        tanqueActual,
        ultimoNodo,
        nodoRandom,
        caminoRandom,
        tamanoRandom
    );

    for (int i = 1; i < tamanoRandom; i++) {
        if (tamanoCamino < Grafo::totalNodos) {
            camino[tamanoCamino] = caminoRandom[i];
            tamanoCamino++;
        }
    }

    finRandom = tamanoCamino - 1;

    // If the random correction did not move, stop here.
    // This avoids a strange second try from the same blocked cell.
    if (finRandom == finIntento1) {
        finIntento2 = finRandom;
        return tamanoCamino > 1;
    }

    int nodoSegundoInicio = camino[tamanoCamino - 1];

    int caminoSegundoIntento[Grafo::totalNodos];
    int tamanoSegundoIntento = 0;

    // Second try: line view again to the original target
    rutalineavist(
        grafo,
        map,
        tanques,
        tanqueActual,
        nodoSegundoInicio,
        nodoDestino,
        caminoSegundoIntento,
        tamanoSegundoIntento
    );

    for (int i = 1; i < tamanoSegundoIntento; i++) {
        if (tamanoCamino < Grafo::totalNodos) {
            camino[tamanoCamino] = caminoSegundoIntento[i];
            tamanoCamino++;
        }
    }

    finIntento2 = tamanoCamino - 1;

    return tamanoCamino > 1;
}