#include "HUD.h"
#include <string>

HUD::HUD() {
}

void HUD::draw(sf::RenderWindow& ventana, sf::Font& font, listaTank& tanques, TurnManager& turnManager, GameTimer& gameTimer) {
	float hudX = 615.0f;
	float hudY = 20.0f;

	sf::Text title(font);
	title.setString("Tank Attack");
	title.setCharacterSize(22);
	title.setFillColor(sf::Color::White);
	title.setPosition({ hudX, hudY });

	ventana.draw(title);

	sf::Text turno(font);

	turno.setString(
		"Turno:" + std::to_string(turnManager.getTurnNumber())
	);
	
	turno.setCharacterSize(18);
	turno.setFillColor(sf::Color::White);
	turno.setPosition({ hudX, hudY + 45.0f });

	ventana.draw(turno);

	std::string timetext = "Tiempo restante: " + std::to_string(gameTimer.getMinutes()) + ":";

	if (gameTimer.getSeconds() < 10){
		timetext += "0";
	
	}

	timetext += std::to_string(gameTimer.getSeconds());

	sf::Text vidasTitulo(font);
	vidasTitulo.setString("Vidas");
	vidasTitulo.setCharacterSize(18);
	vidasTitulo.setFillColor(sf::Color::White);
	vidasTitulo.setPosition({ hudX, hudY + 125.0f });

	ventana.draw(vidasTitulo);

	tanques.drawlife(ventana, font, hudX, hudY + 160.0f);


}