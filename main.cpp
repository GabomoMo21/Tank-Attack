#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <random>

class Nodocola {
public:
    int fila;
    int columna;
    Nodocola* next;

    Nodocola(int fila, int columna) {
        this->fila = fila;
        this->columna = columna;
        this->next = nullptr;
    }
};

class NodoColaEntero {
public:
    int dato;
	NodoColaEntero* next;

    NodoColaEntero(int dato) {
        this->dato = dato;
        this->next = nullptr;
	}
};

class Cola {
private:
    Nodocola* front;
    Nodocola* end;

public:
    Cola() {
        front = nullptr;
        end = nullptr;
    }

    ~Cola() {
        while (!empty()) {
            dequeue();
        }
    }

    bool empty() {
        return front == nullptr;
    }

    void enqueue(int fila, int columna) {
        Nodocola* nuevoNodo = new Nodocola(fila, columna);

        if (empty()) {
            front = nuevoNodo;
            end = nuevoNodo;
        }
        else {
            end->next = nuevoNodo;
            end = nuevoNodo;
        }
    }

    void dequeue() {
        if (empty()) {
            return;
        }

        Nodocola* temp = front;
        front = front->next;

        if (front == nullptr) {
            end = nullptr;
        }

        delete temp;
    }

    int getFrontFila() {
        if (empty()) {
            return -1;
        }

        return front->fila;
    }

    int getFrontColumna() {
        if (empty()) {
            return -1;
        }

        return front->columna;
    }
};


class ColaEnteros {
    private:
        NodoColaEntero* front;
		NodoColaEntero* end;

public:
    ColaEnteros() {
        front = nullptr;
        end = nullptr;
	}

    ~ColaEnteros() {
        while (!empty()) {
            dequeue();
        }
    }

        bool empty() {
            return front == nullptr;
        }

        void enqueue(int dato) {
            NodoColaEntero* nuevoNodo = new NodoColaEntero(dato);

            if (empty()) {
                front = nuevoNodo;
                end = nuevoNodo;
            }
            else {
                end->next = nuevoNodo;
                end = nuevoNodo;
            }
        }

        void dequeue() {
            if (empty()) {
                return;
            }

            NodoColaEntero* temp = front;
            front = front->next;

            if (front == nullptr) {
                end = nullptr;
            }

            delete temp;
        }

        int getFront() {
            if (empty()) {
                return -1;
            }

            return front->dato;
        }

};


class RandomGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;

public:
    RandomGenerator() : gen(rd()) {}

    int randomEntero(int minimo, int maximo) {
        std::uniform_int_distribution<> distribucion(minimo, maximo);
        return distribucion(gen);
    }
};

class Mapa {
public:
    static const int fil = 15;
    static const int col = 15;

    int m[fil][col];

    Mapa() {
        for (int i = 0; i < fil; i++) {
            for (int j = 0; j < col; j++) {
                m[i][j] = 0;
            }
        }
    }

    bool recorrible(int valor) {
        return valor == 0 || valor == 2 || valor == 3;
    }

    void generarmaparandom(RandomGenerator& random) {
        for (int i = 0; i < fil; i++) {
            for (int j = 0; j < col; j++) {
                if (i == 0 || i == fil - 1 || j == 0 || j == col - 1) {
                    m[i][j] = 1;
                }
                else {
                    m[i][j] = 0;
                }
            }
        }

        for (int i = 1; i < fil - 1; i++) {
            for (int j = 1; j < col - 1; j++) {

                if ((i == 1 && j == 1) || (i == fil - 2 && j == col - 2)) {
                    continue;
                }

                if (random.randomEntero(1, 100) <= 25) {
                    m[i][j] = 1;
                }
            }
        }

        m[1][1] = 2;
        m[fil - 2][col - 2] = 3;
    }
};

class Grafo {
public:
    static const int totalNodos = Mapa::fil * Mapa::col;

    int adyacencia[totalNodos][totalNodos];

    Grafo() {
        for (int i = 0; i < totalNodos; i++) {
            for (int j = 0; j < totalNodos; j++) {
                adyacencia[i][j] = 0;
            }
        }
    }

    int obtenerNodo(int fila, int columna) {
        return fila * Mapa::col + columna;
    }

    int obtenerFila(int nodo) {
        return nodo / Mapa::col;
	}

    int obtenerColumna(int nodo) {
        return nodo % Mapa::col;
	}



    void generarMatrizadyacencia(Mapa& map) {
        for (int i = 0; i < totalNodos; i++) {
            for (int j = 0; j < totalNodos; j++) {
                adyacencia[i][j] = 0;
            }
        }

        int df[4] = { -1, 1, 0, 0 };
        int dc[4] = { 0, 0, -1, 1 };

        for (int fila = 0; fila < Mapa::fil; fila++) {
            for (int columna = 0; columna < Mapa::col; columna++) {

                if (map.recorrible(map.m[fila][columna])) {
                    int nodoActual = obtenerNodo(fila, columna);

                    for (int k = 0; k < 4; k++) {
                        int nuevaFila = fila + df[k];
                        int nuevaColumna = columna + dc[k];

                        if (
                            nuevaFila >= 0 &&
                            nuevaFila < Mapa::fil &&
                            nuevaColumna >= 0 &&
                            nuevaColumna < Mapa::col &&
                            map.recorrible(map.m[nuevaFila][nuevaColumna])
                        ) {
                            int nodoVecino = obtenerNodo(nuevaFila, nuevaColumna);

                            adyacencia[nodoActual][nodoVecino] = 1;
                            adyacencia[nodoVecino][nodoActual] = 1;
                        }
                    }
                }
            }
        }
    }
};

class Pathfinder {
public:

    int buscarNodoValor(Mapa& map, Grafo& grafo, int valorBuscado) {
        for (int i = 0; i < Mapa::fil; i++) {
            for (int j = 0; j < Mapa::col;j++) {
                if(map.m[i][j] == valorBuscado){
					return grafo.obtenerNodo(i, j);
                }
        }
    }
		return -1;
    }

    bool buscarRutaBFS(
        Mapa& map,
        Grafo& grafo,
        int valorInicio,
        int valorDestino,
        int camino[],
        int& tamanoCamino
    ) {
        int inicio = buscarNodoValor(map, grafo, valorInicio);
        int destino = buscarNodoValor(map, grafo, valorDestino);

        if (inicio == -1 || destino == -1) {
            return false;
        }

        bool visitado[Grafo::totalNodos] = {};
        int anterior[Grafo::totalNodos];

        for (int i = 0; i < Grafo::totalNodos; i++) {
            anterior[i] = -1;
        }

        ColaEnteros cola;

        cola.enqueue(inicio);
        visitado[inicio] = true;

        while (!cola.empty()) {
            int actual = cola.getFront();
            cola.dequeue();

            if (actual == destino) {
                break;
            }

            for (int vecino = 0; vecino < Grafo::totalNodos; vecino++) {
                if (
                    grafo.adyacencia[actual][vecino] == 1 &&
                    !visitado[vecino]
                    ) {
                    visitado[vecino] = true;
                    anterior[vecino] = actual;
                    cola.enqueue(vecino);
                }
            }
        }

        if (!visitado[destino]) {
            return false;
        }

        int caminoInvertido[Grafo::totalNodos];
        int cantidad = 0;

        int nodoActual = destino;

        while (nodoActual != -1) {
            caminoInvertido[cantidad] = nodoActual;
            cantidad++;

            nodoActual = anterior[nodoActual];
        }

        tamanoCamino = cantidad;

        for (int i = 0; i < cantidad; i++) {
            camino[i] = caminoInvertido[cantidad - 1 - i];
        }

        return true;
    }

    bool validacionBFS(Mapa& map) {
        bool visitado[Mapa::fil][Mapa::col] = {};

        int totalRecorrible = 0;
        int visitados = 0;

        int inicioFila = -1;
        int inicioColumna = -1;

        for (int i = 0; i < Mapa::fil; i++) {
            for (int j = 0; j < Mapa::col; j++) {
                if (map.recorrible(map.m[i][j])) {
                    totalRecorrible++;

                    if (inicioFila == -1) {
                        inicioFila = i;
                        inicioColumna = j;
                    }
                }
            }
        }

        if (inicioFila == -1) {
            return false;
        }

        Cola cola;

        cola.enqueue(inicioFila, inicioColumna);
        visitado[inicioFila][inicioColumna] = true;

        while (!cola.empty()) {
            int fila = cola.getFrontFila();
            int columna = cola.getFrontColumna();

            cola.dequeue();

            visitados++;

            int df[4] = { -1, 1, 0, 0 };
            int dc[4] = { 0, 0, -1, 1 };

            for (int k = 0; k < 4; k++) {
                int nuevaFila = fila + df[k];
                int nuevaColumna = columna + dc[k];

                if (
                    nuevaFila >= 0 &&
                    nuevaFila < Mapa::fil &&
                    nuevaColumna >= 0 &&
                    nuevaColumna < Mapa::col &&
                    !visitado[nuevaFila][nuevaColumna] &&
                    map.recorrible(map.m[nuevaFila][nuevaColumna])
                ) {
                    visitado[nuevaFila][nuevaColumna] = true;
                    cola.enqueue(nuevaFila, nuevaColumna);
                }
            }
        }

        return visitados == totalRecorrible;
    }
};

void dibujarRuta(
    sf::RenderWindow& ventana,
    Grafo& grafo,
    int camino[],
    int tamanoCamino,
    float tamanoCelda
) {
    for (int i = 1; i < tamanoCamino - 1; i++) {
        int nodo = camino[i];

        int fila = grafo.obtenerFila(nodo);
        int columna = grafo.obtenerColumna(nodo);

        sf::RectangleShape celdaRuta;
        celdaRuta.setSize({ tamanoCelda, tamanoCelda });

        celdaRuta.setPosition({
            columna * tamanoCelda,
            fila * tamanoCelda
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
    float tamanoCelda
) {
    sf::Sprite sprite(textura);

    sprite.setPosition({
        columna * tamanoCelda,
        fila * tamanoCelda
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
    sf::Texture& tanque1,
    sf::Texture& tanque2,
    float tamanoCelda
) {
    for (int i = 0; i < Mapa::fil; i++) {
        for (int j = 0; j < Mapa::col; j++) {

            if (map.m[i][j] == 0) {
                dibujarcelda(ventana, suelo, i, j, tamanoCelda);
            }
            else if (map.m[i][j] == 1) {
                dibujarcelda(ventana, pared, i, j, tamanoCelda);
            }
            else if (map.m[i][j] == 2) {
                dibujarcelda(ventana, tanque1, i, j, tamanoCelda);
            }
            else if (map.m[i][j] == 3) {
                dibujarcelda(ventana, tanque2, i, j, tamanoCelda);
            }
        }
    }
}

int main()
{
    RandomGenerator random;
    Mapa map;
    Grafo grafo;
    Pathfinder pathfinder;

    do {
        map.generarmaparandom(random);
    } while (!pathfinder.validacionBFS(map));

    grafo.generarMatrizadyacencia(map);

    std::cout << map.m[1][1] << std::endl;


    int camino[Grafo::totalNodos];
    int tamanoCamino = 0;

    bool hayRuta = pathfinder.buscarRutaBFS(
        map,
        grafo,
        2,
        3,
        camino,
        tamanoCamino
    );

    sf::RenderWindow mapa(
        sf::VideoMode({ 800u, 600u }),
        "Mapa",
        sf::Style::Resize
    );

    sf::Texture suelo("suelo.png");
    sf::Texture pared("pared.png");
    sf::Texture tanque1("tanque1.png");
    sf::Texture tanque2("tanque2.png");

    const float tamanoCelda = 40.f;

    while (mapa.isOpen()) {
        while (const std::optional event = mapa.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                mapa.close();
            }
        }

        mapa.clear(sf::Color::Black);

        dibujarmapa(mapa, map, suelo, pared, tanque1, tanque2, tamanoCelda);

        if (hayRuta) {
            dibujarRuta(mapa, grafo, camino, tamanoCamino, tamanoCelda);
        }


        mapa.display();
    }

    return 0;
}