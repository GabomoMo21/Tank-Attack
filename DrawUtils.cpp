#include "DrawUtils.h"

void dibujarRuta(
    sf::RenderWindow& ventana,
    Grafo& grafo,
    int camino[],
    int tamanoCamino,
    float tamanoCelda,
    float offsetX,
    float offsetY
) {
    for (int i = 1; i < tamanoCamino - 1; i++) {
        int nodo = camino[i];

        int fila = grafo.obtenerFila(nodo);
        int columna = grafo.obtenerColumna(nodo);

        sf::RectangleShape celdaRuta;
        celdaRuta.setSize({ tamanoCelda, tamanoCelda });

        celdaRuta.setPosition({
            offsetX + columna * tamanoCelda,
            offsetY + fila * tamanoCelda
            });

        celdaRuta.setFillColor(sf::Color(255, 255, 0, 120));

        ventana.draw(celdaRuta);
    }
}

void dibujarcelda(
    sf::RenderWindow& ventana,
    sf::Texture& textura,
    int fila,
    int columna,
    float tamanoCelda,
    float offsetX,
    float offsetY
) {
    sf::Sprite sprite(textura);

    sprite.setPosition({
        offsetX + columna * tamanoCelda,
        offsetY + fila * tamanoCelda
        });

    sprite.setScale({
        tamanoCelda / textura.getSize().x,
        tamanoCelda / textura.getSize().y
        });

    ventana.draw(sprite);
}

void dibujarmapa(
    sf::RenderWindow& ventana,
    Mapa& map,
    sf::Texture& suelo,
    sf::Texture& pared,
    float tamanoCelda,
    float offsetX,
    float offsetY
) {
    for (int i = 0; i < Mapa::fil; i++) {
        for (int j = 0; j < Mapa::col; j++) {

            if (map.m[i][j] == 0) {
                dibujarcelda(
                    ventana,
                    suelo,
                    i,
                    j,
                    tamanoCelda,
                    offsetX,
                    offsetY
                );
            }
            else if (map.m[i][j] == 1) {
                dibujarcelda(
                    ventana,
                    pared,
                    i,
                    j,
                    tamanoCelda,
                    offsetX,
                    offsetY
                );
            }
        }
    }
}

void dibujartank(
    sf::RenderWindow& ventana,
    Tank& tank,
    float tamanoCelda,
    float offsetX,
    float offsetY
) {
    if (!tank.estavivo()) {
        return;
    }

    float x = offsetX + tank.getVisualColumna() * tamanoCelda;
    float y = offsetY + tank.getVisualFila() * tamanoCelda;

    sf::RectangleShape cuerpo;
    cuerpo.setSize({ tamanoCelda * 0.8f, tamanoCelda * 0.8f });
    cuerpo.setPosition({ x + tamanoCelda * 0.1f, y + tamanoCelda * 0.1f });
    cuerpo.setFillColor(tank.getcolor());

    sf::RectangleShape canon;
    canon.setSize({ tamanoCelda * 0.45f, tamanoCelda * 0.15f });
    canon.setOrigin({ 0.0f, tamanoCelda * 0.075f });
    canon.setPosition({
        x + tamanoCelda * 0.5f,
        y + tamanoCelda * 0.5f
        });
    canon.setRotation(sf::degrees(tank.getCannonAngle()));
    canon.setFillColor(sf::Color::Black);

    ventana.draw(cuerpo);
    ventana.draw(canon);
    // This draws the life bar background
    sf::RectangleShape lifeBack;
    lifeBack.setSize({ tamanoCelda * 0.8f, 5.0f });
    lifeBack.setPosition({ x + tamanoCelda * 0.1f, y - 7.0f });
    lifeBack.setFillColor(sf::Color::Red);

    // This draws the current life
    float lifeWidth = (float(tank.getvida()) / 100.0f) * (tamanoCelda * 0.8f);

    sf::RectangleShape lifeBar;
    lifeBar.setSize({ lifeWidth, 5.0f });
    lifeBar.setPosition({ x + tamanoCelda * 0.1f, y - 7.0f });
    lifeBar.setFillColor(sf::Color::Green);

    ventana.draw(lifeBack);
    ventana.draw(lifeBar);
}
void dibujarRutaRandom(
    sf::RenderWindow& ventana,
    Grafo& grafo,
    int camino[],
    int tamanoCamino,
    int finIntento1,
    int finRandom,
    int finIntento2,
    float tamanoCelda,
    float offsetX,
    float offsetY
) {
    for (int i = 1; i < tamanoCamino; i++) {
        int nodo = camino[i];

        int fila = grafo.obtenerFila(nodo);
        int columna = grafo.obtenerColumna(nodo);

        sf::RectangleShape celdaRuta;
        celdaRuta.setSize({ tamanoCelda, tamanoCelda });

        celdaRuta.setPosition({
            offsetX + columna * tamanoCelda,
            offsetY + fila * tamanoCelda
            });

        // First try: white
        if (i <= finIntento1) {
            celdaRuta.setFillColor(sf::Color(255, 255, 255, 120));
        }
        // Random correction: yellow
        else if (i <= finRandom) {
            celdaRuta.setFillColor(sf::Color(255, 255, 0, 130));
        }
        // Second try: green
        else {
            celdaRuta.setFillColor(sf::Color(0, 255, 0, 130));
        }

        ventana.draw(celdaRuta);
    }
}
void dibujarLineaVista(
    sf::RenderWindow& ventana,
    Grafo& grafo,
    int nodoInicio,
    int nodoFin,
    float tamanoCelda,
    float offsetX,
    float offsetY,
    sf::Color color
) {
    if (nodoInicio == -1 || nodoFin == -1) {
        return;
    }

    int filaInicio = grafo.obtenerFila(nodoInicio);
    int columnaInicio = grafo.obtenerColumna(nodoInicio);

    int filaFin = grafo.obtenerFila(nodoFin);
    int columnaFin = grafo.obtenerColumna(nodoFin);

    float x1 = offsetX + columnaInicio * tamanoCelda + tamanoCelda / 2.0f;
    float y1 = offsetY + filaInicio * tamanoCelda + tamanoCelda / 2.0f;

    float x2 = offsetX + columnaFin * tamanoCelda + tamanoCelda / 2.0f;
    float y2 = offsetY + filaFin * tamanoCelda + tamanoCelda / 2.0f;

    sf::VertexArray line(sf::PrimitiveType::Lines, 2);

    line[0].position = { x1, y1 };
    line[0].color = color;

    line[1].position = { x2, y2 };
    line[1].color = color;

    ventana.draw(line);
}