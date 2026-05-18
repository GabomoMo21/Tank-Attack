#include "Game.h"

Game::Game()
    : gameTimer(300.0f),
    window(sf::VideoMode({ 800u, 660u }), "Tank Attack", sf::Style::Resize),
    tankselected(nullptr),
    tamanoCamino(0),
    hayRuta(false),
    tamanoCelda(40.0f),
    headerAltura(60.0f),
    mapaOffsetX(0.0f),
    mapaOffsetY(60.0f)
{
}

int Game::run() {
    inicializarTanques();

    generarMapaValido();

    if (!cargarAssets()) {
        return -1;
    }

    while (window.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();

        revisarFinDeJuego();
        procesarEventos();
        actualizar(deltaTime);
        actualizarTitulo();
        dibujar();
    }

    return 0;
}

void Game::inicializarTanques() {
    tanques.agregar(TankFactory::creartanqueazul(1, 1));
    tanques.agregar(TankFactory::creartanqueazul(1, 2));
    tanques.agregar(TankFactory::crearTanqueRojo(2, 1));
    tanques.agregar(TankFactory::crearTanqueRojo(2, 2));

    tanques.agregar(TankFactory::crearTanqueCeleste(13, 13));
    tanques.agregar(TankFactory::crearTanqueCeleste(13, 12));
    tanques.agregar(TankFactory::crearTanqueAmarillo(12, 13));
    tanques.agregar(TankFactory::crearTanqueAmarillo(12, 12));
}

bool Game::cargarAssets() {
    if (!suelo.loadFromFile("suelo.png")) {
        std::cout << "Error loading suelo.png\n";
        return false;
    }

    if (!pared.loadFromFile("pared.png")) {
        std::cout << "Error loading pared.png\n";
        return false;
    }

    if (!font.openFromFile("PixelifySans-VariableFont_wght.ttf")) {
        std::cout << "Error loading font\n";
        return false;
    }

    return true;
}

void Game::generarMapaValido() {
    do {
        map.generarmaparandom(random);
    } while (!pathfinder.validacionBFS(map));

    grafo.generarMatrizadyacencia(map);
}

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
    if (
        keyPressed.code == sf::Keyboard::Key::LShift ||
        keyPressed.code == sf::Keyboard::Key::RShift
        ) {
        if (!bullet.isActive()) {
            int player = turnManager.getActualPlayer();

            powerUpManager.consumePowerUp(player, turnManager);

            tankselected = nullptr;
            hayRuta = false;
            tamanoCamino = 0;
            bullet.clearTrail();

            // Si el power-up debe consumir turno, activa esto:
            // finalizarAccion();
        }
    }
}

void Game::procesarMouse(const sf::Event::MouseButtonPressed& mouseButton) {
    if (bullet.isActive()) {
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

void Game::seleccionarTanque(int fila, int columna) {
    tankselected = tanques.buscarTanqueEnCelda(fila, columna);

    if (tankselected != nullptr) {
        if (turnManager.isPlayerTurn(tankselected->getplayer())) {
            hayRuta = false;
            tamanoCamino = 0;
            bullet.clearTrail();
        }
        else {
            tankselected = nullptr;
        }
    }
}

void Game::moverTanqueSeleccionado(int filaDestino, int columnaDestino) {
    if (
        !map.recorrible(map.m[filaDestino][columnaDestino]) ||
        tanques.celdaOcupada(filaDestino, columnaDestino)
        ) {
        return;
    }

    int nodoInicio = grafo.obtenerNodo(
        tankselected->getfila(),
        tankselected->getcolumna()
    );

    int nodoDestino = grafo.obtenerNodo(
        filaDestino,
        columnaDestino
    );

    int chance = random.randomEntero(1, 100);
    int player = tankselected->getplayer();

    int typeOneLimit = 50;
    int typeTwoLimit = 80;

    if (powerUpManager.hasMovePrecision(player)) {
        typeOneLimit = 90;
        typeTwoLimit = 90;
    }

    if (tankselected->gettipo() == 1) {
        if (chance <= typeOneLimit) {
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
            hayRuta = pathfinder.rutalineavistrandom(
                grafo,
                map,
                tanques,
                tankselected,
                nodoInicio,
                nodoDestino,
                camino,
                tamanoCamino,
                4,
                random
            );
        }
    }
    else {
        if (chance <= typeTwoLimit) {
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
            hayRuta = pathfinder.rutalineavistrandom(
                grafo,
                map,
                tanques,
                tankselected,
                nodoInicio,
                nodoDestino,
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

        powerUpManager.useMovePrecision(player);

        finalizarAccion();
    }

    tankselected = nullptr;
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

void Game::dispararTanqueSeleccionado(int targetRow, int targetCol) {
    int player = tankselected->getplayer();

    bool fullPower = powerUpManager.hasAttackPower(player);
    bool attackPrecision = powerUpManager.hasAttackPrecision(player);

    if (attackPrecision) {
        int nodoInicio = grafo.obtenerNodo(
            tankselected->getfila(),
            tankselected->getcolumna()
        );

        int nodoDestino = grafo.obtenerNodo(
            targetRow,
            targetCol
        );

        hayRuta = pathfinder.buscarRutaAStar(
            grafo,
            tanques,
            tankselected,
            nodoInicio,
            nodoDestino,
            camino,
            tamanoCamino
        );

        if (hayRuta && tamanoCamino > 0) {
            bullet.shootAStar(
                tankselected,
                camino,
                tamanoCamino,
                tamanoCelda,
                fullPower,
                grafo
            );

            powerUpManager.useAttackPrecision(player);
            powerUpManager.useAttackPower(player);

            tankselected = nullptr;

            finalizarAccion();
        }
    }
    else {
        bullet.shoot(
            tankselected,
            targetRow,
            targetCol,
            tamanoCelda,
            fullPower
        );

        powerUpManager.useAttackPower(player);

        hayRuta = false;
        tankselected = nullptr;

        finalizarAccion();
    }
}

void Game::finalizarAccion() {
    int jugadorQueTermino = turnManager.getActualPlayer();

    tryGenerateRandomPowerUp(jugadorQueTermino);

    turnManager.nextTurn();
}

void Game::tryGenerateRandomPowerUp(int jugador) {
    int chance = random.randomEntero(1, 100);

    if (chance <= 80) {
        powerUpManager.addRandomPowerUp(jugador, random);
    }
}

void Game::actualizar(float deltaTime) {
    bullet.update(deltaTime, map, tanques, tamanoCelda);
}

void Game::revisarFinDeJuego() {
    int player1Alive = tanques.contarVivosJugador(1);
    int player2Alive = tanques.contarVivosJugador(2);

    if (player1Alive == 0) {
        std::cout << "Player 2 wins\n";
        window.close();
        return;
    }

    if (player2Alive == 0) {
        std::cout << "Player 1 wins\n";
        window.close();
        return;
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

        window.close();
    }
}

void Game::actualizarTitulo() {
    int player1Alive = tanques.contarVivosJugador(1);
    int player2Alive = tanques.contarVivosJugador(2);

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

    window.setTitle(titulo);
}

void Game::dibujar() {
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

    window.display();
}