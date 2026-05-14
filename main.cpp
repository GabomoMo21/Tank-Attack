#include <SFML/Graphics.hpp>
#include <optional>
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
#include <string>
#include "GameTimer.h"
#include <string>


int main()
{
    RandomGenerator random;
    Mapa map;
    Grafo grafo;
    Pathfinder pathfinder;
    listaTank tanques;
	TurnManager turnManager;
	GameTimer gameTimer(300.0f);

    tanques.agregar(TankFactory::creartanqueazul(1, 1));
    tanques.agregar(TankFactory::creartanqueazul(1, 2));
    tanques.agregar(TankFactory::crearTanqueRojo(2, 1));
    tanques.agregar(TankFactory::crearTanqueRojo(2, 2));

    tanques.agregar(TankFactory::crearTanqueCeleste(13, 13));
    tanques.agregar(TankFactory::crearTanqueCeleste(13, 12));
    tanques.agregar(TankFactory::crearTanqueAmarillo(12, 13));
    tanques.agregar(TankFactory::crearTanqueAmarillo(12, 12));

    Tank* tankselected = nullptr;

    int camino[Grafo::totalNodos];
    int tamanoCamino = 0;
    bool hayRuta = false;

    Bullet bullet;

    do {
        map.generarmaparandom(random);
    } while (!pathfinder.validacionBFS(map));

    grafo.generarMatrizadyacencia(map);

    sf::RenderWindow mapa(
        sf::VideoMode({ 800u, 600u }),
        "Mapa",
        sf::Style::Resize
    );

    sf::Clock gameClock;

    sf::Texture suelo;
    sf::Texture pared;

    if (!suelo.loadFromFile("suelo.png")) {
        std::cout << "Error loading suelo.png\n";
    }

    if (!pared.loadFromFile("pared.png")) {
        std::cout << "Error loading pared.png\n";
    }

    const float tamanoCelda = 40.0f;

    while (mapa.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();

        int player1Alive = tanques.contarVivosJugador(1);
        int player2Alive = tanques.contarVivosJugador(2);

        if (player1Alive == 0) {
            std::cout << "Player 2 wins\n";
            mapa.close();
        }

        if (player2Alive == 0) {
            std::cout << "Player 1 wins\n";
            mapa.close();
        }

        if (gameTimer.TimeUp()) {
            int winner = tanques.getWinnerByAliveTanks();

            if (winner == 1) {
                std::cout << "Player 1 wins by time\n";
            }
            else if (winner == 2) {
                std::cout << "Player 2 wins by time\n";
            }
            else {
                std::cout << "Draw\n";
            }

            mapa.close();
        }

        while (const std::optional event = mapa.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                mapa.close();
            }

            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    int columnaDestino = int(mouseButton->position.x / tamanoCelda);
                    int filaDestino = int(mouseButton->position.y / tamanoCelda);

                    if (
                        filaDestino >= 0 &&
                        filaDestino < Mapa::fil &&
                        columnaDestino >= 0 &&
                        columnaDestino < Mapa::col
                        ) {

                        if (tankselected == nullptr) {
                            tankselected = tanques.buscarTanqueEnCelda(
                                filaDestino,
                                columnaDestino
                            );

                            if (tankselected != nullptr) {
                                if(turnManager.isPlayerTurn(tankselected->getplayer())){
                                    hayRuta = false;
                                    tamanoCamino = 0;
                                }
                                else {
									tankselected = nullptr;
                                }
                            }
                        }
                        else {
                            if (
                                map.recorrible(map.m[filaDestino][columnaDestino]) &&
                                !tanques.celdaOcupada(filaDestino, columnaDestino)
                                ) {
                                int nodoInicio = grafo.obtenerNodo(
                                    tankselected->getfila(),
                                    tankselected->getcolumna()
                                );

                                int nodoDestino = grafo.obtenerNodo(
                                    filaDestino,
                                    columnaDestino
                                );

                                int chance = random.randomEntero(1, 100);

                                if (tankselected->gettipo() == 1) {
                                    if (chance <= 50) {
                                        // Blue and cyan tanks use BFS with 50 percent
                                        hayRuta = pathfinder.buscarRutaBFS(
                                            grafo,
                                            tanques,
											tankselected,
                                            nodoInicio,
                                            nodoDestino,
                                            camino,
                                            tamanoCamino
                                        );
                                    }
                                    else {
                                        // Blue and cyan tanks use random movement with 50 percent
                                        hayRuta = pathfinder.buscarRutaRandom(
                                            grafo,
                                            map,
                                            tanques,
                                            tankselected,
                                            nodoInicio,
                                            camino,
                                            tamanoCamino,
                                            4,
                                            random
                                        );
                                    }
                                }
                                else {
                                    if (chance <= 80) {
                                        // Red and yellow tanks use Dijkstra with 80 percent
                                        hayRuta = pathfinder.buscarRutaDijkstra(
                                            grafo,
                                            tanques,
                                            tankselected,
                                            nodoInicio,
                                            nodoDestino,
                                            camino,
                                            tamanoCamino
                                        );
                                    }
                                    else {
                                        // Red and yellow tanks use random movement with 20 percent
                                        hayRuta = pathfinder.buscarRutaRandom(
                                            grafo,
                                            map,
                                            tanques,
                                            tankselected,
                                            nodoInicio,
                                            camino,
                                            tamanoCamino,
                                            4,
                                            random
                                        );
                                    }
                                }

                                if (hayRuta && tamanoCamino > 0) {
                                    int nodoFinal = camino[tamanoCamino - 1];

                                    int nuevaFila = grafo.obtenerFila(nodoFinal);
                                    int nuevaColumna = grafo.obtenerColumna(nodoFinal);

                                    tankselected->move(nuevaFila, nuevaColumna);

									turnManager.nextTurn();
                                }

                                tankselected = nullptr;
                            }
                        }
                    }
                }

                if (mouseButton->button == sf::Mouse::Button::Right) {
                    int targetCol = int(mouseButton->position.x / tamanoCelda);
                    int targetRow = int(mouseButton->position.y / tamanoCelda);

                    if (
                        targetRow >= 0 &&
                        targetRow < Mapa::fil &&
                        targetCol >= 0 &&
                        targetCol < Mapa::col
                        ) {
                        if (tankselected != nullptr) {
                            bullet.shoot(
                                tankselected,
                                targetRow,
                                targetCol,
                                tamanoCelda
                            );

                            hayRuta = false;
                            tankselected = nullptr;

							turnManager.nextTurn();
                        }
                    }
                }
            }
        }

        
        // This moves the bullet every frame
        bullet.update(deltaTime, map, tanques, tamanoCelda);

        std::string titulo =
            "Tank Attack - Player " +
            std::to_string(turnManager.getActualPlayer()) +
            " | Time: " +
            std::to_string(gameTimer.getMinutes()) +
            ":";

        if (gameTimer.getSeconds() < 10) {
            titulo += "0";
        }

        titulo += std::to_string(gameTimer.getSeconds());

        titulo +=
            " | P1 tanks: " +
            std::to_string(player1Alive) +
            " | P2 tanks: " +
            std::to_string(player2Alive);

        mapa.setTitle(titulo);

        mapa.clear(sf::Color::Black);

        dibujarmapa(mapa, map, suelo, pared, tamanoCelda);

        if (hayRuta) {
            dibujarRuta(mapa, grafo, camino, tamanoCamino, tamanoCelda);
        }

        tanques.dibujarTodos(mapa, tamanoCelda);

        bullet.draw(mapa);

        mapa.display();
}

    return 0;
}   