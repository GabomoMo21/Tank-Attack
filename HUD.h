#pragma once
#include <SFML/Graphics.hpp>
#include "TankList.h"
#include "TurnManager.h"
#include "GameTimer.h"

class HUD {
public:
	HUD();

	void draw(sf::RenderWindow& ventana, sf::Font& font, listaTank& tanques, TurnManager& turnManager, GameTimer& gameTimer);
};