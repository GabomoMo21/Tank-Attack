#include "Pathfinder.h"
#include "TankList.h"
#include "Tank.h"

bool Pathfinder::buscarRutaRandom(
    Grafo& grafo,
    Mapa& map,
    listaTank& tanques,
    Tank* tanqueActual,
    int nodoInicio,
    int camino[],
    int& tamanoCamino,
    int radio,
    RandomGenerator& random
) {
    if (nodoInicio == -1) {
        tamanoCamino = 0;
        return false;
    }

    int startRow = grafo.obtenerFila(nodoInicio);
    int startCol = grafo.obtenerColumna(nodoInicio);

    int tries = 20;

    for (int i = 0; i < tries; i++) {
        int randomRow = startRow + random.randomEntero(-radio, radio);
        int randomCol = startCol + random.randomEntero(-radio, radio);

        if (
            randomRow >= 0 &&
            randomRow < Mapa::fil &&
            randomCol >= 0 &&
            randomCol < Mapa::col &&
            map.recorrible(map.m[randomRow][randomCol]) &&
            !tanques.cellOccupiedExcept(randomRow, randomCol, tanqueActual)
            ) {
            int nodoDestino = grafo.obtenerNodo(randomRow, randomCol);

            return buscarRutaBFS(
                grafo,
                tanques,
                tanqueActual,
                nodoInicio,
                nodoDestino,
                camino,
                tamanoCamino
            );
        }
    }

    tamanoCamino = 0;
    return false;
}