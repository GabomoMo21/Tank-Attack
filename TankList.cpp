#include "TankList.h"
#include "DrawUtils.h"

NodoTank::NodoTank(Tank tank) : tank(tank) {
    next = nullptr;
}

listaTank::listaTank() {
    head = nullptr;
}

listaTank::~listaTank() {
    while (head != nullptr) {
        NodoTank* temp = head;
        head = head->next;
        delete temp;
    }
}

void listaTank::agregar(Tank tank) {
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

Tank* listaTank::buscarTanqueEnCelda(int fila, int columna) {
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

bool listaTank::celdaOcupada(int fila, int columna) {
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
int listaTank::contarVivosJugador(int jugador) {
    int count = 0;

    NodoTank* actual = head;

    while (actual != nullptr) {
        if (
            actual->tank.estavivo() &&
            actual->tank.getplayer() == jugador
            ) {
            count++;
        }

        actual = actual->next;
    }

    return count;
}

int listaTank::getWinnerByAliveTanks() {
    int player1Tanks = contarVivosJugador(1);
    int player2Tanks = contarVivosJugador(2);

    if (player1Tanks > player2Tanks) {
        return 1;
    }

    if (player2Tanks > player1Tanks) {
        return 2;
    }

    return 0;
}
void listaTank::dibujarTodos(sf::RenderWindow& ventana, float tamanoCelda) {
    NodoTank* actual = head;

    while (actual != nullptr) {
        dibujartank(ventana, actual->tank, tamanoCelda);
        actual = actual->next;
    }
}

void listaTank::drawlife(
    sf::RenderWindow& ventana, sf::Font& font, float x, float y
) {
	NodoTank* actual = head;

    int contador = 1;
	float separacion = 28.0f;

    while (actual != nullptr) {
        std::string texto = 
            "J" + std::to_string(actual-> tank.getplayer()) +
			"T" + std::to_string(contador) +
			":" + std::to_string(actual->tank.getvida());

        if (!actual->tank.estavivo()) {
            texto += "x";
        }

		sf::Text vidaTexto(font);
		vidaTexto.setString(texto);
		vidaTexto.setCharacterSize(15);
		vidaTexto.setFillColor(actual->tank.getcolor());
		vidaTexto.setPosition({ x, y });

		ventana.draw(vidaTexto);

		y += separacion;
		contador++;

		actual = actual->next;
    
    }

}

bool listaTank::cellOccupiedExcept(int fila, int columna, Tank* tanqueignorado) {
    NodoTank* actual = head;

    while (actual != nullptr) {
        if (
            actual->tank.estavivo() &&
            actual->tank.incell(fila, columna) &&
            &(actual->tank) != tanqueignorado
            ) {
            return true;
        }

        actual = actual->next;
    }

    return false;
}