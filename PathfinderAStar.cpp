#include "Pathfinder.h"
#include "TankList.h"
#include "Tank.h"
#include <cstdlib>

bool Pathfinder::buscarRutaAStar(
    Grafo& grafo,
    listaTank& tanques,
    Tank* tanqueActual,
    int nodoInicio,
    int nodoDestino,
    int camino[],
    int& tamanoCamino
) {
    bool visitado[Grafo::totalNodos];
    int costo[Grafo::totalNodos];
    int padre[Grafo::totalNodos];

    for (int i = 0; i < Grafo::totalNodos; i++) {
        visitado[i] = false;
        costo[i] = 999999;
        padre[i] = -1;
    }

    costo[nodoInicio] = 0;

    for (int paso = 0; paso < Grafo::totalNodos; paso++) {
        int nodoActual = -1;
        int mejorValor = 999999;

        for (int i = 0; i < Grafo::totalNodos; i++) {
            if (!visitado[i] && costo[i] < 999999) {
                int filaActual = grafo.obtenerFila(i);
                int columnaActual = grafo.obtenerColumna(i);

                int filaDestino = grafo.obtenerFila(nodoDestino);
                int columnaDestino = grafo.obtenerColumna(nodoDestino);

                int distancia =
                    abs(filaActual - filaDestino) +
                    abs(columnaActual - columnaDestino);

                int valorAStar = costo[i] + distancia;

                if (valorAStar < mejorValor) {
                    mejorValor = valorAStar;
                    nodoActual = i;
                }
            }
        }

        if (nodoActual == -1) {
            break;
        }

        if (nodoActual == nodoDestino) {
            break;
        }

        visitado[nodoActual] = true;

        for (int vecino = 0; vecino < Grafo::totalNodos; vecino++) {
            if (grafo.adyacencia[nodoActual][vecino] > 0 && !visitado[vecino]) {
                int filaVecino = grafo.obtenerFila(vecino);
                int columnaVecino = grafo.obtenerColumna(vecino);

                bool ocupado = tanques.celdaOcupada(filaVecino, columnaVecino);

                if (vecino == nodoDestino) {
                    ocupado = false;
                }

                if (tanqueActual != nullptr) {
                    if (
                        filaVecino == tanqueActual->getfila() &&
                        columnaVecino == tanqueActual->getcolumna()
                        ) {
                        ocupado = false;
                    }
                }

                if (!ocupado) {
                    int peso = grafo.adyacencia[nodoActual][vecino];
                    int nuevoCosto = costo[nodoActual] + peso;

                    if (nuevoCosto < costo[vecino]) {
                        costo[vecino] = nuevoCosto;
                        padre[vecino] = nodoActual;
                    }
                }
            }
        }
    }

    if (nodoInicio != nodoDestino && padre[nodoDestino] == -1) {
        tamanoCamino = 0;
        return false;
    }

    int caminoTemporal[Grafo::totalNodos];
    int tamanoTemporal = 0;

    int actual = nodoDestino;

    while (actual != -1) {
        caminoTemporal[tamanoTemporal] = actual;
        tamanoTemporal++;

        if (actual == nodoInicio) {
            break;
        }

        actual = padre[actual];
    }

    tamanoCamino = 0;

    for (int i = tamanoTemporal - 1; i >= 0; i--) {
        camino[tamanoCamino] = caminoTemporal[i];
        tamanoCamino++;
    }

    return true;
}