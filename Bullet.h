#pragma once

#include <SFML/Graphics.hpp>

class Mapa;
class Tank;
class listaTank;

class Bullet {
private:
    float x;
    float y;
    float speed;

    float dirX;
    float dirY;

    bool active;

    int bounceCount;
    int maxBounces;

    Tank* owner;
    bool canHitOwner;

    float trailX[400];
    float trailY[400];
    int trailSize;

public:
    Bullet();

    bool isActive();

    void clearTrail();
    void addTrailPoint();

    void shoot(Tank* shooter, int targetRow, int targetCol, float cellSize);
    void update(float deltaTime, Mapa& map, listaTank& tanks, float cellSize);
    void draw(sf::RenderWindow& window, float offsetX, float offsetY);
       
    void deactivate();
};