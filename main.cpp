#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <random>
#include <cmath>

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
        return valor == 0;
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

                bool isSpawnCell =
                    (i == 1 && j == 1) ||
                    (i == 1 && j == 2) ||
                    (i == 2 && j == 1) ||
                    (i == 2 && j == 2) ||
                    (i == fil - 2 && j == col - 2) ||
                    (i == fil - 2 && j == col - 3) ||
                    (i == fil - 3 && j == col - 2) ||
                    (i == fil - 3 && j == col - 3);

                if (isSpawnCell) {
                    continue;
                }

                if (random.randomEntero(1, 100) <= 25) {
                    m[i][j] = 1;
                }
            }
        }
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

    bool buscarRutaBFS(
        Grafo& grafo,
        int nodoInicio,
        int nodoDestino,
        int camino[],
        int& tamanoCamino
    ) {
        if (nodoInicio == -1 || nodoDestino == -1) {
            tamanoCamino = 0;
            return false;
        }

        bool visitado[Grafo::totalNodos] = {};
        int anterior[Grafo::totalNodos];

        for (int i = 0; i < Grafo::totalNodos; i++) {
            anterior[i] = -1;
        }

        ColaEnteros cola;

        cola.enqueue(nodoInicio);
        visitado[nodoInicio] = true;

        while (!cola.empty()) {
            int actual = cola.getFront();
            cola.dequeue();

            if (actual == nodoDestino) {
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

        if (!visitado[nodoDestino]) {
            tamanoCamino = 0;
            return false;
        }

        int caminoInvertido[Grafo::totalNodos];
        int cantidad = 0;

        int nodoActual = nodoDestino;

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



class Tank {
public:
    int fila;
    int columna;
    int tipo;
	int jugador;
    int vida;
    bool vivo;
    sf::Color color;

    Tank(int fila, int columna, int tipo, int jugador, sf::Color color) {
        this->fila = fila;
        this->columna = columna;
        this->tipo = tipo;
        this->jugador = jugador;
        this->vida = 100;
		this->vivo = true;
        this->color = color;

    }

    int getfila() {
        return fila;
    }

    int getcolumna() {
        return columna;
    }

    int gettipo() {
        return tipo;
    }

    int getvida() {
        return vida;
	}

    bool estavivo() {
        return vivo;
	}

    sf::Color getcolor() {
        return color;
	}


    void move(int nuevaFila, int nuevaColumna) {
        fila = nuevaFila;
        columna = nuevaColumna;
    }

    bool incell(int filaclick, int columnaclick) {
        return fila == filaclick && columna == columnaclick;
    }

    void getdamage(int damage) {
        vida -= damage;
        if (vida <= 0) {
            vida = 0;
            vivo = false;
        }
	}
};

class TankFactory {
public:
    static Tank creartanqueazul(int fila, int columna) {
        return Tank(fila, columna, 1, 1, sf::Color::Blue);
    }

    static Tank crearTanqueRojo(int fila, int columna) {
        return Tank(fila, columna, 2, 1, sf::Color::Red);
    }

    static Tank crearTanqueCeleste(int fila, int columna) {
        return Tank(fila, columna, 1, 2, sf::Color::Cyan);
    }

    static Tank crearTanqueAmarillo(int fila, int columna) {
        return Tank(fila, columna, 2, 2, sf::Color::Yellow);
    }
};


bool tankincell(Tank& tank1,
    Tank& tank2,
    int fila,
    int columna
) {
    return tank1.incell(fila, columna) || tank2.incell(fila, columna);
}

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
        }
    }
}

void dibujartank(
    sf::RenderWindow& ventana,
    Tank& tank,
    float tamanoCelda
) {
    if (!tank.estavivo()) {
        return;
    }

	float x = tank.getcolumna() * tamanoCelda;
	float y = tank.getfila() * tamanoCelda;

    sf::RectangleShape cuerpo;
    cuerpo.setSize({ tamanoCelda * 0.8f, tamanoCelda * 0.8f });
    cuerpo.setPosition({ x + tamanoCelda * 0.1f, y + tamanoCelda * 0.1f });
    cuerpo.setFillColor(tank.getcolor());

    sf::RectangleShape canon;
    canon.setSize({ tamanoCelda * 0.15f, tamanoCelda * 0.45f });
    canon.setPosition({ x + tamanoCelda * 0.425f, y });
    canon.setFillColor(sf::Color::Black);

    ventana.draw(cuerpo);
    ventana.draw(canon);

}

class NodoTank {
public:
    Tank tank;
    NodoTank* next;

    NodoTank(Tank tank) : tank(tank) {
        next = nullptr;
    }
};


class listaTank {
private:
    NodoTank* head;

public:
    listaTank() {
        head = nullptr;
    }

    ~listaTank() {
        while (head != nullptr) {
            NodoTank* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void agregar(Tank tank) {
        NodoTank* nuevo = new NodoTank(tank);


        if (head == nullptr) {
            head = nuevo;
        }

        else {
            NodoTank* actual = head;

            while (actual->next != nullptr) {
                actual = actual->next;
            }

            actual->next = nuevo;

        }
    }

    Tank* buscarTanqueEnCelda(int fila, int columna) {
        NodoTank* actual = head;

        while (actual != nullptr) {
            if (
                actual->tank.estavivo() &&
                actual->tank.incell(fila, columna)
                ) {
                return &(actual->tank);
            }

            actual = actual->next;
        }

        return nullptr;
    }


    bool celdaOcupada(int fila, int columna) {
        NodoTank* actual = head;

        while (actual != nullptr) {
            if (
                actual->tank.estavivo() &&
                actual->tank.incell(fila, columna)
                ) {
                return true;
            }

            actual = actual->next;
        }

        return false;
    }

    void dibujarTodos(sf::RenderWindow& ventana, float tamanoCelda) {
        NodoTank* actual = head;

        while (actual != nullptr) {
            dibujartank(ventana, actual->tank, tamanoCelda);
            actual = actual->next;
        }
    }
};


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
    Bullet() {
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

        for (int i = 0; i < 400; i++) {
            trailX[i] = 0.0f;
            trailY[i] = 0.0f;
        }
    }

    bool isActive() {
        return active;
    }

    void clearTrail() {
        trailSize = 0;

        for (int i = 0; i < 400; i++) {
            trailX[i] = 0.0f;
            trailY[i] = 0.0f;
        }
    }

    void addTrailPoint() {
        if (trailSize < 400) {
            trailX[trailSize] = x;
            trailY[trailSize] = y;
            trailSize++;
        }
    }

    void shoot(Tank* shooter, int targetRow, int targetCol, float cellSize) {
        if (shooter == nullptr) {
            return;
        }

        owner = shooter;
        active = true;

        bounceCount = 0;
        canHitOwner = false;

        clearTrail();

    
        x = shooter->getcolumna() * cellSize + cellSize / 2.0f;
        y = shooter->getfila() * cellSize + cellSize / 2.0f;

    
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

    void update(float deltaTime, Mapa& map, listaTank& tanks, float cellSize) {
        if (!active) {
            return;
        }

        float nextX = x + dirX * speed * deltaTime;
        float nextY = y + dirY * speed * deltaTime;

        int currentRow = int(y / cellSize);
        int currentCol = int(x / cellSize);

        int nextRow = int(nextY / cellSize);
        int nextCol = int(nextX / cellSize);

    
        if (
            nextRow < 0 ||
            nextRow >= Mapa::fil ||
            nextCol < 0 ||
            nextCol >= Mapa::col
            ) {
            active = false;
            return;
        }

    
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


            if (owner != nullptr && owner->gettipo() == 2) {
                damage = 50;
            }

            hitTank->getdamage(damage);

            active = false;
        }
    }

    void draw(sf::RenderWindow& window) {
     
        for (int i = 0; i < trailSize; i++) {
            sf::CircleShape point;
            point.setRadius(3.0f);
            point.setOrigin({ 3.0f, 3.0f });
            point.setPosition({ trailX[i], trailY[i] });
            point.setFillColor(sf::Color(230, 230, 230));
            window.draw(point);
        }

        if (!active) {
            return;
        }

       
        sf::CircleShape bulletShape;
        bulletShape.setRadius(6.0f);
        bulletShape.setOrigin({ 6.0f, 6.0f });
        bulletShape.setPosition({ x, y });
        bulletShape.setFillColor(sf::Color::White);

        window.draw(bulletShape);
    }
};



int main()
{
    RandomGenerator random;
    Mapa map;
    Grafo grafo;
    Pathfinder pathfinder;
    listaTank tanques;

    tanques.agregar(TankFactory::creartanqueazul(1, 1));
    tanques.agregar(TankFactory::creartanqueazul(1, 2));
    tanques.agregar(TankFactory::crearTanqueRojo(2, 1));
    tanques.agregar(TankFactory::crearTanqueRojo(2, 2));

    tanques.agregar(TankFactory::crearTanqueCeleste(13, 13));
    tanques.agregar(TankFactory::crearTanqueCeleste(13, 12));
    tanques.agregar(TankFactory::crearTanqueAmarillo(12, 13));
    tanques.agregar(TankFactory::crearTanqueAmarillo(12, 12));

    Tank* tankselected = nullptr;

    int camino[Grafo::totalNodos];
    int tamanoCamino = 0;
    bool hayRuta = false;
    Bullet bullet;    

    do {
        map.generarmaparandom(random);
    } while (!pathfinder.validacionBFS(map));

    grafo.generarMatrizadyacencia(map);

    sf::RenderWindow mapa(
        sf::VideoMode({ 800u, 600u }),
        "Mapa",
        sf::Style::Resize
    );
    sf::Clock gameClock;

    sf::Texture suelo("suelo.png");
    sf::Texture pared("pared.png");
 

    const float tamanoCelda = 40.f;

    while (mapa.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();
        while (const std::optional event = mapa.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                mapa.close();
            }

            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {

                if (mouseButton->button == sf::Mouse::Button::Left) {

                    int columnaDestino = int(mouseButton->position.x / tamanoCelda);
                    int filaDestino = int(mouseButton->position.y / tamanoCelda);

                    if (
                        filaDestino >= 0 &&
                        filaDestino < Mapa::fil &&
                        columnaDestino >= 0 &&
                        columnaDestino < Mapa::col
                        ) {

                        if (tankselected == nullptr) {

                            if (tankselected == nullptr) {
                                tankselected = tanques.buscarTanqueEnCelda(filaDestino, columnaDestino);

                                if (tankselected != nullptr) {
                                    hayRuta = false;
                                    tamanoCamino = 0;
                                }
                            }

                        }
                        else {

                            if (
                                map.recorrible(map.m[filaDestino][columnaDestino]) &&
                                !tanques.celdaOcupada(filaDestino, columnaDestino)
                                ) {

                                int nodoInicio = grafo.obtenerNodo(
                                    tankselected->getfila(),
                                    tankselected->getcolumna()
                                );

                                int nodoDestino = grafo.obtenerNodo(
                                    filaDestino,
                                    columnaDestino
                                );

                                hayRuta = pathfinder.buscarRutaBFS(
                                    grafo,
                                    nodoInicio,
                                    nodoDestino,
                                    camino,
                                    tamanoCamino
                                );

                                if (hayRuta && tamanoCamino > 0) {
                                    int nodoFinal = camino[tamanoCamino - 1];

                                    int nuevaFila = grafo.obtenerFila(nodoFinal);
                                    int nuevaColumna = grafo.obtenerColumna(nodoFinal);

                                    tankselected->move(nuevaFila, nuevaColumna);
                                }

                                tankselected = nullptr;
                            }
                        }
                    }
                }
                if (mouseButton->button == sf::Mouse::Button::Right) {
                    int targetCol = int(mouseButton->position.x / tamanoCelda);
                    int targetRow = int(mouseButton->position.y / tamanoCelda);

                    if (
                        targetRow >= 0 &&
                        targetRow < Mapa::fil &&
                        targetCol >= 0 &&
                        targetCol < Mapa::col
                        ) {
                        if (tankselected != nullptr) {
                            bullet.shoot(
                                tankselected,
                                targetRow,
                                targetCol,
                                tamanoCelda
                            );

                            hayRuta = false;
                            tankselected = nullptr;
                        }
                    }
                }
            }
        }
        bullet.update(deltaTime, map, tanques, tamanoCelda);

        mapa.clear(sf::Color::Black);

        dibujarmapa(mapa, map, suelo, pared, tamanoCelda);

        if (hayRuta) {
            dibujarRuta(mapa, grafo, camino, tamanoCamino, tamanoCelda);
        }

        tanques.dibujarTodos(mapa, tamanoCelda);
        bullet.draw(mapa);
        mapa.display();
    }

    return 0;
}