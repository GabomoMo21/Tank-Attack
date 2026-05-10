#include "Grafo.h"

Grafo::Grafo() {
    for (int i = 0; i < totalNodos; i++) {
        for (int j = 0; j < totalNodos; j++) {
            adyacencia[i][j] = 0;
        }
    }
}

int Grafo::obtenerNodo(int fila, int columna) {
    return fila * Mapa::col + columna;
}

int Grafo::obtenerFila(int nodo) {
    return nodo / Mapa::col;
}

int Grafo::obtenerColumna(int nodo) {
    return nodo % Mapa::col;
}

void Grafo::generarMatrizadyacencia(Mapa& map) {
    for (int i = 0; i < totalNodos; i++) {
        for (int j = 0; j < totalNodos; j++) {
            adyacencia[i][j] = 0;
        }
    }

    int df[4] = { -1, 1, 0, 0 };
    int dc[4] = { 0, 0, -1, 1 };

    for (int fila = 0; fila < Mapa::fil; fila++) {
        for (int columna = 0; columna < Mapa::col; columna++) {
            if (map.recorrible(map.m[fila][columna])) {
                int nodoActual = obtenerNodo(fila, columna);

                for (int k = 0; k < 4; k++) {
                    int nuevaFila = fila + df[k];
                    int nuevaColumna = columna + dc[k];

                    if (
                        nuevaFila >= 0 &&
                        nuevaFila < Mapa::fil &&
                        nuevaColumna >= 0 &&
                        nuevaColumna < Mapa::col &&
                        map.recorrible(map.m[nuevaFila][nuevaColumna])
                        ) {
                        int nodoVecino = obtenerNodo(nuevaFila, nuevaColumna);

                        adyacencia[nodoActual][nodoVecino] = 1;
                        adyacencia[nodoVecino][nodoActual] = 1;
                    }
                }
            }
        }
    }
}