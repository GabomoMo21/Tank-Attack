#include "Game.h"
#include <cmath>

void Game::dibujar() {
    if (gameOver) {
        dibujarPantallaVictoria();
        return;
    }

    window.clear(sf::Color::Black);

    hud.draw(
        window,
        font,
        tanques,
        turnManager,
        gameTimer,
        powerUpManager
    );

    dibujarmapa(
        window,
        map,
        suelo,
        pared,
        tamanoCelda,
        mapaOffsetX,
        mapaOffsetY
    );

    if (hayRuta) {
        if (rutaRandomVisible) {
            dibujarLineaVista(
                window,
                grafo,
                vistaInicio1,
                vistaFin1,
                tamanoCelda,
                mapaOffsetX,
                mapaOffsetY,
                sf::Color(255, 255, 255, 180)
            );

            dibujarLineaVista(
                window,
                grafo,
                vistaInicio2,
                vistaFin2,
                tamanoCelda,
                mapaOffsetX,
                mapaOffsetY,
                sf::Color(0, 255, 0, 180)
            );

            dibujarRutaRandom(
                window,
                grafo,
                camino,
                tamanoCamino,
                finIntento1,
                finRandom,
                finIntento2,
                tamanoCelda,
                mapaOffsetX,
                mapaOffsetY
            );
        }
        else {
            dibujarRuta(
                window,
                grafo,
                camino,
                tamanoCamino,
                tamanoCelda,
                mapaOffsetX,
                mapaOffsetY
            );
        }
    }

    tanques.dibujarTodos(
        window,
        tamanoCelda,
        mapaOffsetX,
        mapaOffsetY
    );

    bullet.draw(
        window,
        mapaOffsetX,
        mapaOffsetY
    );

    dibujarBotones();

    window.display();
}

void Game::dibujarPantallaVictoria() {
    window.clear(sf::Color::Black);

    float pulse = 1.0f + 0.08f * std::sin(victoryAnimTime * 4.0f);

    sf::RectangleShape fondo;
    fondo.setSize({ 800.0f, 660.0f });
    fondo.setPosition({ 0.0f, 0.0f });
    fondo.setFillColor(sf::Color(20, 20, 30));
    window.draw(fondo);

    std::string textoVictoria;

    if (ganador == 1) {
        textoVictoria = "PLAYER 1 WINS";
    }
    else if (ganador == 2) {
        textoVictoria = "PLAYER 2 WINS";
    }
    else {
        textoVictoria = "DRAW";
    }

    sf::Text titulo(font);
    titulo.setString(textoVictoria);
    titulo.setCharacterSize(unsigned int(42 * pulse));
    titulo.setFillColor(sf::Color::White);

    sf::FloatRect bounds = titulo.getLocalBounds();
    titulo.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
        });

    titulo.setPosition({ 400.0f, 250.0f });

    window.draw(titulo);

    sf::Text subtitulo(font);
    subtitulo.setString("Press ENTER or ESC to exit");
    subtitulo.setCharacterSize(20);
    subtitulo.setFillColor(sf::Color(200, 200, 200));

    sf::FloatRect subBounds = subtitulo.getLocalBounds();
    subtitulo.setOrigin({
        subBounds.position.x + subBounds.size.x / 2.0f,
        subBounds.position.y + subBounds.size.y / 2.0f
        });

    subtitulo.setPosition({ 400.0f, 330.0f });

    window.draw(subtitulo);

    for (int i = 0; i < 12; i++) {
        float angle = victoryAnimTime * 2.0f + i * 0.52f;
        float radius = 120.0f + 20.0f * std::sin(victoryAnimTime * 3.0f + i);

        float x = 400.0f + std::cos(angle) * radius;
        float y = 250.0f + std::sin(angle) * radius;

        sf::CircleShape circle;
        circle.setRadius(5.0f);
        circle.setOrigin({ 5.0f, 5.0f });
        circle.setPosition({ x, y });

        if (ganador == 1) {
            circle.setFillColor(sf::Color::Blue);
        }
        else if (ganador == 2) {
            circle.setFillColor(sf::Color::Cyan);
        }
        else {
            circle.setFillColor(sf::Color::Yellow);
        }

        window.draw(circle);
    }

    window.display();
}

void Game::dibujarBotones() {
    sf::RectangleShape muteShape;
    muteShape.setSize({ 70.0f, 30.0f });
    muteShape.setPosition({ 620.0f, 610.0f });
    muteShape.setFillColor(sf::Color(40, 40, 40));
    muteShape.setOutlineColor(sf::Color::White);
    muteShape.setOutlineThickness(1.0f);
    window.draw(muteShape);

    sf::Text muteText(font);

    if (audioManager.isMuted()) {
        muteText.setString("UNMUTE");
        muteText.setCharacterSize(12);
        muteText.setPosition({ 624.0f, 616.0f });
    }
    else {
        muteText.setString("MUTE");
        muteText.setCharacterSize(13);
        muteText.setPosition({ 633.0f, 616.0f });
    }

    muteText.setFillColor(sf::Color::White);
    window.draw(muteText);

    sf::RectangleShape exitShape;
    exitShape.setSize({ 70.0f, 30.0f });
    exitShape.setPosition({ 710.0f, 610.0f });
    exitShape.setFillColor(sf::Color(40, 40, 40));
    exitShape.setOutlineColor(sf::Color::White);
    exitShape.setOutlineThickness(1.0f);
    window.draw(exitShape);

    sf::Text exitText(font);
    exitText.setString("EXIT");
    exitText.setCharacterSize(13);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition({ 725.0f, 616.0f });
    window.draw(exitText);
}