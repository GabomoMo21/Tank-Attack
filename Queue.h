#pragma once

class Nodocola {
public:
    int fila;
    int columna;
    Nodocola* next;

    Nodocola(int fila, int columna);
};

class NodoColaEntero {
public:
    int dato;
    NodoColaEntero* next;

    NodoColaEntero(int dato);
};

class Cola {
private:
    Nodocola* front;
    Nodocola* end;

public:
    Cola();
    ~Cola();

    bool empty();
    void enqueue(int fila, int columna);
    void dequeue();

    int getFrontFila();
    int getFrontColumna();
};

class ColaEnteros {
private:
    NodoColaEntero* front;
    NodoColaEntero* end;

public:
    ColaEnteros();
    ~ColaEnteros();

    bool empty();
    void enqueue(int dato);
    void dequeue();

    int getFront();
};