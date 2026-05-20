#include "Pathfinder.h"
#include "Queue.h"
#include "TankList.h"
#include "Tank.h"
#include <cmath>
#include <cstdlib>

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

                int distancia = abs(filaActual - filaDestino) + abs(columnaActual - columnaDestino);

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