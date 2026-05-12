#include "GameTimer.h"

GameTimer::GameTimer(float deltaTime){
	this->deltaTime = deltaTime;
}

float GameTimer::getElapsedTime() {
	return clock.getElapsedTime().asSeconds();
}

float GameTimer::getTimeLeft() {
	float left = deltaTime - getElapsedTime();

	if(left < 0.0f){
		left = 0.0f;
	}

	return left;
}

bool GameTimer::TimeUp(){
	return getTimeLeft() <= 0.0f;
}
int GameTimer::getMinutes() {
	return int(getTimeLeft()) / 60;
}

int GameTimer::getSeconds() {
	return int(getTimeLeft()) % 60;
}

