#include "HUD.h"
#include <string>

HUD::HUD() {
}

void HUD::draw(
    sf::RenderWindow& ventana,
    sf::Font& font,
    listaTank& tanques,
    TurnManager& turnManager,
    GameTimer& gameTimer
) {
    float headerH = 60.0f;

    float mapW = 600.0f;
    float hudX = 600.0f;
    float hudW = 200.0f;
    float windowH = 660.0f;

    float mitadHudX = hudX + hudW / 2.0f;
    float hudContenidoY = headerH;
    float powerY = headerH + 300.0f;

    sf::Color lineColor = sf::Color::Black;
    sf::Color textColor = sf::Color::White;

    // Fondo blanco del HUD completo
    sf::RectangleShape fondoHUD;
    fondoHUD.setSize({ 800.0f, 660.0f });
    fondoHUD.setPosition({ 0.0f, 0.0f });
    fondoHUD.setFillColor(sf::Color::Black);
    ventana.draw(fondoHUD);

    // Área del juego en blanco, luego el mapa se dibuja encima
    sf::RectangleShape areaJuego;
    areaJuego.setSize({ mapW, 600.0f });
    areaJuego.setPosition({ 0.0f, headerH });
    areaJuego.setFillColor(sf::Color::White);
    ventana.draw(areaJuego);

    // Línea horizontal debajo del título
    sf::RectangleShape lineaHeader;
    lineaHeader.setSize({ 800.0f, 2.0f });
    lineaHeader.setPosition({ 0.0f, headerH });
    lineaHeader.setFillColor(lineColor);
    ventana.draw(lineaHeader);

    // Línea vertical entre juego y HUD
    sf::RectangleShape lineaVerticalHUD;
    lineaVerticalHUD.setSize({ 2.0f, 660.0f });
    lineaVerticalHUD.setPosition({ hudX, 0.0f });
    lineaVerticalHUD.setFillColor(lineColor);
    ventana.draw(lineaVerticalHUD);

    // Línea vertical que divide J1 y J2
    sf::RectangleShape lineaMitadHUD;
    lineaMitadHUD.setSize({ 2.0f, 600.0f });
    lineaMitadHUD.setPosition({ mitadHudX, headerH });
    lineaMitadHUD.setFillColor(lineColor);
    ventana.draw(lineaMitadHUD);

    // Línea horizontal que divide tanques y power-ups
    sf::RectangleShape lineaPowerups;
    lineaPowerups.setSize({ hudW, 2.0f });
    lineaPowerups.setPosition({ hudX, powerY });
    lineaPowerups.setFillColor(lineColor);
    ventana.draw(lineaPowerups);

    // Título centrado encima del mapa
    sf::Text titulo(font);
    titulo.setString("tank attack");
    titulo.setCharacterSize(28);
    titulo.setFillColor(textColor);
    titulo.setPosition({ 210.0f, 15.0f });
    ventana.draw(titulo);

    // Turno arriba a la derecha
    sf::Text turno(font);
    turno.setString(
        "# turno\nJ" + std::to_string(turnManager.getActualPlayer())
    );
    turno.setCharacterSize(22);
    turno.setFillColor(textColor);
    turno.setPosition({ hudX + 45.0f, 8.0f });
    ventana.draw(turno);

    // Tiempo pequeño debajo del título, opcional
    std::string tiempoTexto =
        "Tiempo " +
        std::to_string(gameTimer.getMinutes()) +
        ":";

    if (gameTimer.getSeconds() < 10) {
        tiempoTexto += "0";
    }

    tiempoTexto += std::to_string(gameTimer.getSeconds());

    sf::Text tiempo(font);
    tiempo.setString(tiempoTexto);
    tiempo.setCharacterSize(14);
    tiempo.setFillColor(textColor);
    tiempo.setPosition({ 20.0f, 20.0f });
    ventana.draw(tiempo);

    // Títulos de jugadores
    sf::Text jugador1(font);
    jugador1.setString("jugador\n1");
    jugador1.setCharacterSize(18);
    jugador1.setFillColor(textColor);
    jugador1.setPosition({ hudX + 20.0f, hudContenidoY + 35.0f });
    ventana.draw(jugador1);

    sf::Text jugador2(font);
    jugador2.setString("jugador\n2");
    jugador2.setCharacterSize(18);
    jugador2.setFillColor(textColor);
    jugador2.setPosition({ hudX + 120.0f, hudContenidoY + 35.0f });
    ventana.draw(jugador2);

    // Vidas de cada jugador
    tanques.drawlife(
        ventana,
        font,
        1,
        hudX + 10.0f,
        hudContenidoY + 115.0f
   
    );

    tanques.drawlife(
        ventana,
        font,
        2,
        hudX + 110.0f,
        hudContenidoY + 115.0f
    );
    // Power-ups vacíos por ahora
    sf::Text power1(font);
    power1.setString("power\nups j1");
    power1.setCharacterSize(18);
    power1.setFillColor(textColor);
    power1.setPosition({ hudX + 10.0f, powerY + 60.0f });
    ventana.draw(power1);

    sf::Text power2(font);
    power2.setString("power\nups j2");
    power2.setCharacterSize(18);
    power2.setFillColor(textColor);
    power2.setPosition({ hudX + 110.0f, powerY + 60.0f });
    ventana.draw(power2);
}