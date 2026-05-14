#include "Pathfinder.h"
#include "Queue.h"
#include "TankList.h"
#include "Tank.h"

bool Pathfinder::buscarRutaBFS(
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
                int filaVecino = grafo.obtenerFila(vecino);
                int columnaVecino = grafo.obtenerColumna(vecino);

                if (!tanques.cellOccupiedExcept(filaVecino, columnaVecino, tanqueActual)) {
                    visitado[vecino] = true;
                    anterior[vecino] = actual;
                    cola.enqueue(vecino);
                }
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

        // This finds the free node with less distance
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

        // This checks all neighbors
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

            // This uses BFS to make a valid route to the random cell
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