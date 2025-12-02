#ifndef DLIST_H
#define DLIST_H

typedef struct Nodo {
    char digito;
    struct Nodo *anterior;
    struct Nodo *siguiente;
} Nodo;

typedef struct {
    Nodo *inicio;
    Nodo *fin;
    int decimales;
    int negativo;
} BigNumber;

BigNumber* crearBigNumber();
void liberarBigNumber(BigNumber *num);
void agregarDigito(BigNumber *num, char digito);
void imprimirBigNumber(BigNumber *num);
BigNumber* stringToBigNumber(const char *str);

#endif
