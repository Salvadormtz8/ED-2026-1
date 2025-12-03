#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void inicializarPila(Pila *pila) {
    pila->cima = -1;
}

int pilaLlena(Pila *pila) {
    return pila->cima == MAX - 1;
}

int pilaVacia(Pila *pila) {
    return pila->cima == -1;
}

void apilar(Pila *pila, const char *elemento) {
    if (pilaLlena(pila)) {
        fprintf(stderr, "Error: La pila está llena.\n");
        exit(EXIT_FAILURE);
    } else {
        pila->cima++;
        strcpy(pila->elementos[pila->cima], elemento);
    }
}

char *desapilar(Pila *pila) {
    static char elemento[MAX];
    if (pilaVacia(pila)) {
        fprintf(stderr, "Error: La pila está vacía.\n");
        return NULL;
    } else {
        strcpy(elemento, pila->elementos[pila->cima]);
        pila->cima--;
        return elemento;
    }
}

char *consultarCima(Pila *pila) {
    static char elemento[MAX];
    if (pilaVacia(pila)) {
        return NULL;
    } else {
        strcpy(elemento, pila->elementos[pila->cima]);
        return elemento;
    }
}

void limpiarPila(Pila *pila) {
    pila->cima = -1;
}
