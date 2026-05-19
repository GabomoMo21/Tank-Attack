#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <iostream>

#include "RandomGenerator.h"
#include "Mapa.h"
#include "Grafo.h"
#include "Pathfinder.h"
#include "Tank.h"
#include "TankFactory.h"
#include "TankList.h"
#include "Bullet.h"
#include "DrawUtils.h"
#include "TurnManager.h"
#include "GameTimer.h"
#include "HUD.h"
#include "PowerUpManager.h"
#include "PowerUp.h"

class Game {
private:
    RandomGenerator random;
    Mapa map;
    Grafo grafo;
    Pathfinder pathfinder;
    listaTank tanques;
    TurnManager turnManager;
    GameTimer gameTimer;
    HUD hud;
    PowerUpManager powerUpManager;
    Bullet bullet;

    sf::RenderWindow window;
    sf::Clock gameClock;

    sf::Texture suelo;
    sf::Texture pared;
    sf::Font font;

    Tank* tankselected;

    int camino[Grafo::totalNodos];
    int tamanoCamino;
    bool hayRuta;

    const float tamanoCelda;
    const float headerAltura;
    const float mapaOffsetX;
    const float mapaOffsetY;

    bool movimientoEnProgreso;
    Tank* movingTank;

    int caminoAnimado[Grafo::totalNodos];
    int tamanoCaminoAnimado;
    int indiceCaminoAnimado;

    int jugadorMovimientoPendiente;

public:
    Game();

    int run();

private:
    void inicializarTanques();
    bool cargarAssets();
    void generarMapaValido();

    void procesarEventos();
    void procesarTecla(const sf::Event::KeyPressed& keyPressed);
    void procesarMouse(const sf::Event::MouseButtonPressed& mouseButton);

    bool clickDentroDelMapa(int mouseX, int mouseY);
    int obtenerFilaMouse(int mouseY);
    int obtenerColumnaMouse(int mouseX);

    void manejarClickIzquierdo(int filaDestino, int columnaDestino);
    void manejarClickDerecho(int targetRow, int targetCol);

    void seleccionarTanque(int fila, int columna);
    void moverTanqueSeleccionado(int filaDestino, int columnaDestino);
    void dispararTanqueSeleccionado(int targetRow, int targetCol);

    void finalizarAccion();

    void tryGenerateRandomPowerUp(int jugador);

    void actualizar(float deltaTime);
    void revisarFinDeJuego();
    void actualizarTitulo();
    void dibujar();

    void iniciarMovimientoAnimado(Tank* tank, int camino[], int tamanoCamino, int jugador);
    void actualizarMovimientoAnimado(float deltaTime);

};