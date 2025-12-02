#include "Stack.h"
#include <string.h>
#include <stdio.h>

void inicializarPila(Pila *p) { p->cima = -1; }
int pilaLlena(Pila *p) { return p->cima == MAX - 1; }
int pilaVacia(Pila *p) { return p->cima == -1; }

void apilar(Pila *p, const char *elemento) {
    if (!pilaLlena(p)) {
        p->cima++;
        strcpy(p->elementos[p->cima], elemento);
    }
}

char *desapilar(Pila *p) {
    return pilaVacia(p) ? NULL : p->elementos[p->cima--];
}

char *consultarCima(Pila *p) {
    return pilaVacia(p) ? NULL : p->elementos[p->cima];
}
