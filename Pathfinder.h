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
    bool buscarRutaRandom(
        Grafo& grafo,
        Mapa& map,
        int nodoInicio,
        int camino[],
        int& tamanoCamino,
        int radio,
        RandomGenerator& random
    );
    bool validacionBFS(Mapa& map);
};