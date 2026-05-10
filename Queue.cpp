#include "Queue.h"

Nodocola::Nodocola(int fila, int columna) {
    this->fila = fila;
    this->columna = columna;
    this->next = nullptr;
}

NodoColaEntero::NodoColaEntero(int dato) {
    this->dato = dato;
    this->next = nullptr;
}

Cola::Cola() {
    front = nullptr;
    end = nullptr;
}

Cola::~Cola() {
    while (!empty()) {
        dequeue();
    }
}

bool Cola::empty() {
    return front == nullptr;
}

void Cola::enqueue(int fila, int columna) {
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

void Cola::dequeue() {
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

int Cola::getFrontFila() {
    if (empty()) {
        return -1;
    }

    return front->fila;
}

int Cola::getFrontColumna() {
    if (empty()) {
        return -1;
    }

    return front->columna;
}

ColaEnteros::ColaEnteros() {
    front = nullptr;
    end = nullptr;
}

ColaEnteros::~ColaEnteros() {
    while (!empty()) {
        dequeue();
    }
}

bool ColaEnteros::empty() {
    return front == nullptr;
}

void ColaEnteros::enqueue(int dato) {
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

void ColaEnteros::dequeue() {
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

int ColaEnteros::getFront() {
    if (empty()) {
        return -1;
    }

    return front->dato;
}