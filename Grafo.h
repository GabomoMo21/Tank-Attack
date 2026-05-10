#pragma once

#include "Mapa.h"

class Grafo {
public:
    static const int totalNodos = Mapa::fil * Mapa::col;

    int adyacencia[totalNodos][totalNodos];

    Grafo();

    int obtenerNodo(int fila, int columna);
    int obtenerFila(int nodo);
    int obtenerColumna(int nodo);

    void generarMatrizadyacencia(Mapa& map);
};