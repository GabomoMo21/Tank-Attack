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