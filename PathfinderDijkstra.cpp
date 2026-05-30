#include "Pathfinder.h"
#include "TankList.h"
#include "Tank.h"

bool Pathfinder::buscarRutaDijkstra(
    Grafo& grafo,
    listaTank& tanques,
    Tank* tanqueActual,
    int nodoInicio,
    int nodoDestino,
    int camino[],
    int& tamanoCamino
) {
    if (nodoInicio == -1 || nodoDestino == -1) {
        tamanoCamino = 0;
        return false;
    }

    int distancia[Grafo::totalNodos];
    bool visitado[Grafo::totalNodos];
    int anterior[Grafo::totalNodos];

    for (int i = 0; i < Grafo::totalNodos; i++) {
        distancia[i] = 999999;
        visitado[i] = false;
        anterior[i] = -1;
    }

    distancia[nodoInicio] = 0;

    for (int i = 0; i < Grafo::totalNodos; i++) {
        int actual = -1;
        int mejorDistancia = 999999;

        for (int j = 0; j < Grafo::totalNodos; j++) {
            if (!visitado[j] && distancia[j] < mejorDistancia) {
                mejorDistancia = distancia[j];
                actual = j;
            }
        }

        if (actual == -1) {
            break;
        }

        if (actual == nodoDestino) {
            break;
        }

        visitado[actual] = true;

        for (int vecino = 0; vecino < Grafo::totalNodos; vecino++) {
            if (grafo.adyacencia[actual][vecino] > 0 && !visitado[vecino]) {
                int filaVecino = grafo.obtenerFila(vecino);
                int columnaVecino = grafo.obtenerColumna(vecino);

                if (!tanques.cellOccupiedExcept(filaVecino, columnaVecino, tanqueActual)) {
                    int peso = grafo.adyacencia[actual][vecino];
                    int nuevaDistancia = distancia[actual] + peso;

                    if (nuevaDistancia < distancia[vecino]) {
                        distancia[vecino] = nuevaDistancia;
                        anterior[vecino] = actual;
                    }
                }
            }
        }
    }

    if (distancia[nodoDestino] == 999999) {
        tamanoCamino = 0;
        return false;
    }

    int caminoInvertido[Grafo::totalNodos];
    int cantidad = 0;

    int nodoActual = nodoDestino;

    while (nodoActual != -1) {
        caminoInvertido[cantidad] = nodoActual;
        cantidad++;

        nodoActual = anterior[nodoActual];
    }

    tamanoCamino = cantidad;

    for (int i = 0; i < cantidad; i++) {
        camino[i] = caminoInvertido[cantidad - 1 - i];
    }

    return true;
}