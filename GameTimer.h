#pragma once

#include <SFML/System.hpp>

class GameTimer {
private:
	sf::Clock clock;
	float deltaTime;


public:
	GameTimer(float deltaTime);

	float getElapsedTime();

	float getTimeLeft();

	bool TimeUp();

	int getMinutes();
	int getSeconds();


};

