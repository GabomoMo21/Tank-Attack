#include "Pathfinder.h"
#include "Queue.h"

bool Pathfinder::buscarRutaBFS(
    Grafo& grafo,
    int nodoInicio,
    int nodoDestino,
    int camino[],
    int& tamanoCamino
) {
    if (nodoInicio == -1 || nodoDestino == -1) {
        tamanoCamino = 0;
        return false;
    }

    bool visitado[Grafo::totalNodos] = {};
    int anterior[Grafo::totalNodos];

    for (int i = 0; i < Grafo::totalNodos; i++) {
        anterior[i] = -1;
    }

    ColaEnteros cola;

    cola.enqueue(nodoInicio);
    visitado[nodoInicio] = true;

    while (!cola.empty()) {
        int actual = cola.getFront();
        cola.dequeue();

        if (actual == nodoDestino) {
            break;
        }

        for (int vecino = 0; vecino < Grafo::totalNodos; vecino++) {
            if (
                grafo.adyacencia[actual][vecino] == 1 &&
                !visitado[vecino]
                ) {
                visitado[vecino] = true;
                anterior[vecino] = actual;
                cola.enqueue(vecino);
            }
        }
    }

    if (!visitado[nodoDestino]) {
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

bool Pathfinder::validacionBFS(Mapa& map) {
    bool visitado[Mapa::fil][Mapa::col] = {};

    int totalRecorrible = 0;
    int visitados = 0;

    int inicioFila = -1;
    int inicioColumna = -1;

    for (int i = 0; i < Mapa::fil; i++) {
        for (int j = 0; j < Mapa::col; j++) {
            if (map.recorrible(map.m[i][j])) {
                totalRecorrible++;

                if (inicioFila == -1) {
                    inicioFila = i;
                    inicioColumna = j;
                }
            }
        }
    }

    if (inicioFila == -1) {
        return false;
    }

    Cola cola;

    cola.enqueue(inicioFila, inicioColumna);
    visitado[inicioFila][inicioColumna] = true;

    while (!cola.empty()) {
        int fila = cola.getFrontFila();
        int columna = cola.getFrontColumna();

        cola.dequeue();

        visitados++;

        int df[4] = { -1, 1, 0, 0 };
        int dc[4] = { 0, 0, -1, 1 };

        for (int k = 0; k < 4; k++) {
            int nuevaFila = fila + df[k];
            int nuevaColumna = columna + dc[k];

            if (
                nuevaFila >= 0 &&
                nuevaFila < Mapa::fil &&
                nuevaColumna >= 0 &&
                nuevaColumna < Mapa::col &&
                !visitado[nuevaFila][nuevaColumna] &&
                map.recorrible(map.m[nuevaFila][nuevaColumna])
                ) {
                visitado[nuevaFila][nuevaColumna] = true;
                cola.enqueue(nuevaFila, nuevaColumna);
            }
        }
    }

    return visitados == totalRecorrible;
}