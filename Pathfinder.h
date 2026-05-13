#pragma once
#include "RandomGenerator.h"
#include "Mapa.h"
#include "Grafo.h"

class Pathfinder {
public:
    bool buscarRutaBFS(
        Grafo& grafo,
        int nodoInicio,
        int nodoDestino,
        int camino[],
        int& tamanoCamino
    );
    bool buscarRutaDijkstra(
        Grafo& grafo,
        int nodoInicio,
        int nodoDestino,
        int camino[],
        int& tamanoCamino
    );

    bool validacionBFS(Mapa& map);
};