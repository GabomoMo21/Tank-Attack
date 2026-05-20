#pragma once

#include <SFML/Graphics.hpp>
#include "Mapa.h"
#include "Grafo.h"
#include "Tank.h"

void dibujarRuta(
    sf::RenderWindow& ventana,
    Grafo& grafo,
    int camino[],
    int tamanoCamino,
    float tamanoCelda,
    float offsetX,
    float offsetY
);

void dibujarcelda(
    sf::RenderWindow& ventana,
    sf::Texture& textura,
    int fila,
    int columna,
    float tamanoCelda,
    float offsetX,
    float offsetY
);

void dibujarmapa(
    sf::RenderWindow& ventana,
    Mapa& map,
    sf::Texture& suelo,
    sf::Texture& pared,
    float tamanoCelda,
    float offsetX,
    float offsetY
);

void dibujartank(
    sf::RenderWindow& ventana,
    Tank& tank,
    float tamanoCelda,
    float offsetX,
    float offsetY
);
void dibujarRutaRandom(
    sf::RenderWindow& ventana,
    Grafo& grafo,
    int camino[],
    int tamanoCamino,
    int finIntento1,
    int finRandom,
    int finIntento2,
    float tamanoCelda,
    float offsetX,
    float offsetY
);
void dibujarLineaVista(
    sf::RenderWindow& ventana,
    Grafo& grafo,
    int nodoInicio,
    int nodoFin,
    float tamanoCelda,
    float offsetX,
    float offsetY,
    sf::Color color
);