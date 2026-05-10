#pragma once

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

    bool validacionBFS(Mapa& map);
};