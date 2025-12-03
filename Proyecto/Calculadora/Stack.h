#ifndef STACK_H
#define STACK_H
#define MAX 1000

typedef struct {
    char elementos[MAX][MAX];
    int cima;
} Pila;

void inicializarPila(Pila *p);
int pilaLlena(Pila *p);
int pilaVacia(Pila *p);
void apilar(Pila *p, const char *elemento);
char *desapilar(Pila *p);
char *consultarCima(Pila *p);
void limpiarPila(Pila *pila);

#endif
