#pragma once

#include <SFML/Graphics.hpp>
#include "Grafo.h"
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
    bool fullPower;
    bool useAStarRoute;

    int routeRows[400];
    int routeCols[400];
    int routeSize;
    int routeIndex;

public:
    Bullet();

    bool isActive();

    void clearTrail();
    void addTrailPoint();

    void shoot(Tank* shooter, int targetRow, int targetCol, float cellSize, bool fullPower);
    void update(float deltaTime, Mapa& map, listaTank& tanks, float cellSize);
    void draw(sf::RenderWindow& window, float offsetX, float offsetY);
       
    void deactivate();
    void shootAStar(
        Tank* shooter,
        int route[],
        int routeSize,
        float cellSize,
        bool fullPower,
        Grafo& grafo
    );
};