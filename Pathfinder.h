#pragma once
#include "RandomGenerator.h"
#include "Mapa.h"
#include "Grafo.h"

class listaTank;
class Tank;

class Pathfinder {
public:
    bool buscarRutaBFS(
        Grafo& grafo,
        listaTank& tanques,
		Tank* tanqueActual,
        int nodoInicio,
        int nodoDestino,
        int camino[],
        int& tamanoCamino
    );
    bool buscarRutaDijkstra(
        Grafo& grafo,
		listaTank& tanques,
		Tank* tanqueActual,
        int nodoInicio,
        int nodoDestino,
        int camino[],
        int& tamanoCamino
    );
    bool buscarRutaRandom(
        Grafo& grafo,
        Mapa& map,
        listaTank& tanques,
        Tank* tanqueActual,
        int nodoInicio,
        int camino[],
        int& tamanoCamino,
        int radio,
        RandomGenerator& random
    );
    bool validacionBFS(Mapa& map);


    bool rutalineavist(
		Grafo& grafo,
		Mapa& map,
		listaTank& tanques,
		Tank* tanqueActual,
		int nodoInicio,
		int nodoDestino,
		int camino[],
		int& tamanoCamino
    );

    bool rutalineavistrandom(
        Grafo& grafo,
        Mapa& map,
        listaTank& tanques,
        Tank* tanqueActual,
        int nodoInicio,
        int nodoDestino,
        int camino[],
        int& tamanoCamino,
		int radio,
		RandomGenerator& random
    );
};