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

void listaTank::dibujarTodos(sf::RenderWindow& ventana, float tamanoCelda) {
    NodoTank* actual = head;

    while (actual != nullptr) {
        dibujartank(ventana, actual->tank, tamanoCelda);
        actual = actual->next;
    }
}