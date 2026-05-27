#include "Game.h"
#include <optional>

void Game::procesarEventos() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            procesarTecla(*keyPressed);
        }

        if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
            procesarMouse(*mouseButton);
        }
    }
}

void Game::procesarTecla(const sf::Event::KeyPressed& keyPressed) {
    if (gameOver) {
        if (
            keyPressed.code == sf::Keyboard::Key::Enter ||
            keyPressed.code == sf::Keyboard::Key::Escape
            ) {
            window.close();
        }

        return;
    }

    if (
        keyPressed.code == sf::Keyboard::Key::LShift ||
        keyPressed.code == sf::Keyboard::Key::RShift
        ) {
        if (!bullet.isActive()) {
            int player = turnManager.getActualPlayer();

            bool consumed = powerUpManager.consumePowerUp(player, turnManager);

            if (consumed) {
                audioManager.playPowerUp();

                tankselected = nullptr;
                hayRuta = false;
                tamanoCamino = 0;
                bullet.clearTrail();
                resetRutaRandomVisual();
            }
        }
    }
}

void Game::procesarMouse(const sf::Event::MouseButtonPressed& mouseButton) {
    if (gameOver) {
        return;
    }

    if (mouseButton.button == sf::Mouse::Button::Left) {
        if (clickEnMute(mouseButton.position.x, mouseButton.position.y)) {
            audioManager.playButton();
            audioManager.toggleMute();
            return;
        }

        if (clickEnExit(mouseButton.position.x, mouseButton.position.y)) {
            audioManager.playButton();
            window.close();
            return;
        }
    }

    if (bullet.isActive() || movimientoEnProgreso) {
        return;
    }

    if (!clickDentroDelMapa(mouseButton.position.x, mouseButton.position.y)) {
        return;
    }

    int columna = obtenerColumnaMouse(mouseButton.position.x);
    int fila = obtenerFilaMouse(mouseButton.position.y);

    if (mouseButton.button == sf::Mouse::Button::Left) {
        manejarClickIzquierdo(fila, columna);
    }
    else if (mouseButton.button == sf::Mouse::Button::Right) {
        manejarClickDerecho(fila, columna);
    }
}

bool Game::clickDentroDelMapa(int mouseX, int mouseY) {
    return
        mouseX >= mapaOffsetX &&
        mouseX < mapaOffsetX + Mapa::col * tamanoCelda &&
        mouseY >= mapaOffsetY &&
        mouseY < mapaOffsetY + Mapa::fil * tamanoCelda;
}

int Game::obtenerFilaMouse(int mouseY) {
    return int((mouseY - mapaOffsetY) / tamanoCelda);
}

int Game::obtenerColumnaMouse(int mouseX) {
    return int((mouseX - mapaOffsetX) / tamanoCelda);
}

void Game::manejarClickIzquierdo(int filaDestino, int columnaDestino) {
    if (
        filaDestino < 0 ||
        filaDestino >= Mapa::fil ||
        columnaDestino < 0 ||
        columnaDestino >= Mapa::col
        ) {
        return;
    }

    if (tankselected == nullptr) {
        seleccionarTanque(filaDestino, columnaDestino);
    }
    else {
        moverTanqueSeleccionado(filaDestino, columnaDestino);
    }
}

void Game::manejarClickDerecho(int targetRow, int targetCol) {
    if (
        targetRow < 0 ||
        targetRow >= Mapa::fil ||
        targetCol < 0 ||
        targetCol >= Mapa::col
        ) {
        return;
    }

    if (tankselected == nullptr) {
        return;
    }

    dispararTanqueSeleccionado(targetRow, targetCol);
}

void Game::seleccionarTanque(int fila, int columna) {
    tankselected = tanques.buscarTanqueEnCelda(fila, columna);

    if (tankselected != nullptr) {
        if (turnManager.isPlayerTurn(tankselected->getplayer())) {
            hayRuta = false;
            tamanoCamino = 0;
            resetRutaRandomVisual();
            bullet.clearTrail();
        }
        else {
            tankselected = nullptr;
        }
    }
}

bool Game::clickEnMute(int mouseX, int mouseY) {
    return
        mouseX >= 620 &&
        mouseX <= 690 &&
        mouseY >= 610 &&
        mouseY <= 640;
}

bool Game::clickEnExit(int mouseX, int mouseY) {
    return
        mouseX >= 710 &&
        mouseX <= 780 &&
        mouseY >= 610 &&
        mouseY <= 640;
}