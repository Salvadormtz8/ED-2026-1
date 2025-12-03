#ifndef DLIST_H
#define DLIST_H

typedef struct Nodo {
    char dato[MAX];
    struct Nodo *siguiente;
    struct Nodo *anterior;
} Nodo;

typedef struct {
    Nodo *cabeza;
    Nodo *cola;
    int tamano;
} ListaCircular;

ListaCircular* crearLista();
void destruirLista(ListaCircular *lista);
void insertarInicio(ListaCircular *lista, const char *dato);
void insertarFinal(ListaCircular *lista, const char *dato);
void eliminarNodo(ListaCircular *lista, Nodo *nodo);
void mostrarLista(ListaCircular *lista);
void guardarEnArchivo(ListaCircular *lista, const char *nombreArchivo);

#endif
