#include "Bullet.h"
#include "Mapa.h"
#include "Tank.h"
#include "TankList.h"

#include <cmath>

Bullet::Bullet() {
    x = 0.0f;
    y = 0.0f;
    speed = 350.0f;

    dirX = 0.0f;
    dirY = 0.0f;

    active = false;

    bounceCount = 0;
    maxBounces = 3;

    owner = nullptr;
    canHitOwner = false;

    trailSize = 0;
    fullPower = false;

    for (int i = 0; i < 400; i++) {
        trailX[i] = 0.0f;
        trailY[i] = 0.0f;
    }
    useAStarRoute = false;
    routeSize = 0;
    routeIndex = 0;

    for (int i = 0; i < 400; i++) {
        routeRows[i] = 0;
        routeCols[i] = 0;
    }   
}

bool Bullet::isActive() {
    return active;
}

void Bullet::clearTrail() {
    trailSize = 0;

    for (int i = 0; i < 400; i++) {
        trailX[i] = 0.0f;
        trailY[i] = 0.0f;
    }
}

void Bullet::addTrailPoint() {
    if (trailSize < 400) {
        trailX[trailSize] = x;
        trailY[trailSize] = y;
        trailSize++;
    }
}

void Bullet::shoot(Tank* shooter, int targetRow, int targetCol, float cellSize, bool fullPower){
    if (shooter == nullptr) {
        return;
    }
    this->fullPower = fullPower;
    owner = shooter;
    active = true;
    useAStarRoute = false;
    routeSize = 0;
    routeIndex = 0;
    killedTank = false;
    bounceCount = 0;
    canHitOwner = false;

    clearTrail();

    // Start in the center of the tank cell
    x = shooter->getcolumna() * cellSize + cellSize / 2.0f;
    y = shooter->getfila() * cellSize + cellSize / 2.0f;

    // Target center
    float targetX = targetCol * cellSize + cellSize / 2.0f;
    float targetY = targetRow * cellSize + cellSize / 2.0f;

    float dx = targetX - x;
    float dy = targetY - y;

    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0.0f) {
        dirX = dx / distance;
        dirY = dy / distance;
    }
    else {
        dirX = 0.0f;
        dirY = -1.0f;
    }

    addTrailPoint();
}
void Bullet::shootAStar(
    Tank* shooter,
    int route[],
    int routeSize,
    float cellSize,
    bool fullPower,
    Grafo& grafo
    
) {
    if (shooter == nullptr) {
        return;
    }

    if (routeSize <= 0) {
        return;
    }

    this->fullPower = fullPower;
    owner = shooter;
    active = true;

    useAStarRoute = true;
    this->routeSize = routeSize;
    routeIndex = 0;

    bounceCount = 0;
    canHitOwner = false;

    clearTrail();

    // Start in the center of the tank cell
    x = shooter->getcolumna() * cellSize + cellSize / 2.0f;
    y = shooter->getfila() * cellSize + cellSize / 2.0f;

    for (int i = 0; i < routeSize && i < 400; i++) {
        routeRows[i] = grafo.obtenerFila(route[i]);
        routeCols[i] = grafo.obtenerColumna(route[i]);
    }

    addTrailPoint();
}

void Bullet::update(float deltaTime, Mapa& map, listaTank& tanks, float cellSize) {
    if (!active) {
        return;
    }
    if (useAStarRoute) {
        if (routeIndex >= routeSize) {
            active = false;
            return;
        }

        float targetX = routeCols[routeIndex] * cellSize + cellSize / 2.0f;
        float targetY = routeRows[routeIndex] * cellSize + cellSize / 2.0f;

        float dx = targetX - x;
        float dy = targetY - y;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 5.0f) {
            routeIndex++;
            return;
        }

        if (distance > 0.0f) {
            dirX = dx / distance;
            dirY = dy / distance;
        }

        x += dirX * speed * deltaTime;
        y += dirY * speed * deltaTime;

        addTrailPoint();

        int bulletRow = int(y / cellSize);
        int bulletCol = int(x / cellSize);

        Tank* hitTank = tanks.buscarTanqueEnCelda(bulletRow, bulletCol);

        if (hitTank != nullptr) {
            if (hitTank == owner && !canHitOwner) {
                if (
                    bulletRow != owner->getfila() ||
                    bulletCol != owner->getcolumna()
                    ) {
                    canHitOwner = true;
                }

                return;
            }

            int damage = 25;

            if (hitTank->gettipo() == 2) {
                damage = 50;
            }

            if (fullPower) {
                damage = 100;
            }

            hitTank->getdamage(damage);
            if (!hitTank->estavivo()) {
                killedTank = true;
            }
            active = false;
        }

        return;
    }

    float nextX = x + dirX * speed * deltaTime;
    float nextY = y + dirY * speed * deltaTime;

    int currentRow = int(y / cellSize);
    int currentCol = int(x / cellSize);

    int nextRow = int(nextY / cellSize);
    int nextCol = int(nextX / cellSize);

    // Check map limits
    if (
        nextRow < 0 ||
        nextRow >= Mapa::fil ||
        nextCol < 0 ||
        nextCol >= Mapa::col
        ) {
        active = false;
        return;
    }

    // The bullet can hit the owner after it leaves the first cell
    if (owner != nullptr) {
        if (
            currentRow != owner->getfila() ||
            currentCol != owner->getcolumna()
            ) {
            canHitOwner = true;
        }
    }

    bool hitWallX = false;
    bool hitWallY = false;

    // Check wall in X movement
    int checkCol = int(nextX / cellSize);

    if (
        currentRow >= 0 &&
        currentRow < Mapa::fil &&
        checkCol >= 0 &&
        checkCol < Mapa::col
        ) {
        if (!map.recorrible(map.m[currentRow][checkCol])) {
            hitWallX = true;
        }
    }

    // Check wall in Y movement
    int checkRow = int(nextY / cellSize);

    if (
        checkRow >= 0 &&
        checkRow < Mapa::fil &&
        currentCol >= 0 &&
        currentCol < Mapa::col
        ) {
        if (!map.recorrible(map.m[checkRow][currentCol])) {
            hitWallY = true;
        }
    }

    // Bounce with walls
    if (hitWallX || hitWallY) {
        if (hitWallX) {
            dirX = -dirX;
        }

        if (hitWallY) {
            dirY = -dirY;
        }

        bounceCount++;

        if (bounceCount > maxBounces) {
            active = false;
        }

        addTrailPoint();
        return;
    }

    // Move bullet
    x = nextX;
    y = nextY;

    addTrailPoint();

    int bulletRow = int(y / cellSize);
    int bulletCol = int(x / cellSize);

    Tank* hitTank = tanks.buscarTanqueEnCelda(bulletRow, bulletCol);

    if (hitTank != nullptr) {
        if (hitTank == owner && !canHitOwner) {
            return;
        }

        int damage = 25;

        if (hitTank->gettipo() == 2) {
            damage = 50;
        }

        if (fullPower) {
            damage = 100;
        }

        hitTank->getdamage(damage);
        if (!hitTank->estavivo()) {
            killedTank = true;
        }
        active = false;
    }
}


void Bullet::draw(sf::RenderWindow& window, float offsetX, float offsetY) {
    // Draw bullet trail
    for (int i = 0; i < trailSize; i++) {
        sf::CircleShape point;
        point.setRadius(3.0f);
        point.setOrigin({ 3.0f, 3.0f });

        point.setPosition({
            offsetX + trailX[i],
            offsetY + trailY[i]
            });

        point.setFillColor(sf::Color(230, 230, 230));

        window.draw(point);
    }

    if (!active) {
        return;
    }

    // Draw bullet
    sf::CircleShape bulletShape;
    bulletShape.setRadius(6.0f);
    bulletShape.setOrigin({ 6.0f, 6.0f });

    bulletShape.setPosition({
        offsetX + x,
        offsetY + y
        });

    bulletShape.setFillColor(sf::Color::White);

    window.draw(bulletShape);
}

void Bullet::deactivate() {
    active = false;
	clearTrail();
}
bool Bullet::didKillTank() {
    if (killedTank) {
        killedTank = false;
        return true;
    }

    return false;
}