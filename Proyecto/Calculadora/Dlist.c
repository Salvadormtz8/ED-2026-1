#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dlist.h"

ListaCircular* crearLista() {
    ListaCircular *lista = (ListaCircular*)malloc(sizeof(ListaCircular));
    if (!lista) {
        perror("Error al crear lista");
        exit(EXIT_FAILURE);
    }
    lista->cabeza = NULL;
    lista->cola = NULL;
    lista->tamano = 0;
    return lista;
}

void destruirLista(ListaCircular *lista) {
    Nodo *actual = lista->cabeza;
    Nodo *temp;
    
    if (actual) {
        do {
            temp = actual;
            actual = actual->siguiente;
            free(temp);
        } while (actual != lista->cabeza);
    }
    
    free(lista);
}

void insertarInicio(ListaCircular *lista, const char *dato) {
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) {
        perror("Error al crear nodo");
        exit(EXIT_FAILURE);
    }
    
    strcpy(nuevo->dato, dato);
    
    if (lista->cabeza == NULL) {
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        lista->cabeza = nuevo;
        lista->cola = nuevo;
    } else {
        nuevo->siguiente = lista->cabeza;
        nuevo->anterior = lista->cola;
        lista->cabeza->anterior = nuevo;
        lista->cola->siguiente = nuevo;
        lista->cabeza = nuevo;
    }
    
    lista->tamano++;
}

void insertarFinal(ListaCircular *lista, const char *dato) {
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) {
        perror("Error al crear nodo");
        exit(EXIT_FAILURE);
    }
    
    strcpy(nuevo->dato, dato);
    
    if (lista->cabeza == NULL) {
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        lista->cabeza = nuevo;
        lista->cola = nuevo;
    } else {
        nuevo->siguiente = lista->cabeza;
        nuevo->anterior = lista->cola;
        lista->cola->siguiente = nuevo;
        lista->cabeza->anterior = nuevo;
        lista->cola = nuevo;
    }
    
    lista->tamano++;
}

void eliminarNodo(ListaCircular *lista, Nodo *nodo) {
    if (!lista || !nodo || lista->tamano == 0) return;
    
    if (lista->tamano == 1) {
        free(nodo);
        lista->cabeza = NULL;
        lista->cola = NULL;
    } else {
        nodo->anterior->siguiente = nodo->siguiente;
        nodo->siguiente->anterior = nodo->anterior;
        
        if (nodo == lista->cabeza)
            lista->cabeza = nodo->siguiente;
        if (nodo == lista->cola)
            lista->cola = nodo->anterior;
        
        free(nodo);
    }
    
    lista->tamano--;
}

void mostrarLista(ListaCircular *lista) {
    if (!lista || lista->tamano == 0) {
        printf("Lista vacía\n");
        return;
    }
    
    Nodo *actual = lista->cabeza;
    int i = 0;
    
    printf("Historial de operaciones:\n");
    do {
        printf("%d. %s\n", ++i, actual->dato);
        actual = actual->siguiente;
    } while (actual != lista->cabeza);
}

void guardarEnArchivo(ListaCircular *lista, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "a");
    if (!archivo) {
        perror("Error al abrir archivo");
        return;
    }
    
    time_t ahora;
    time(&ahora);
    struct tm *local = localtime(&ahora);
    
    fprintf(archivo, "\n=== Registro del %02d/%02d/%04d %02d:%02d ===\n",
            local->tm_mday, local->tm_mon + 1, local->tm_year + 1900,
            local->tm_hour, local->tm_min);
    
    if (!lista || lista->tamano == 0) {
        fprintf(archivo, "No hay operaciones registradas.\n");
    } else {
        Nodo *actual = lista->cabeza;
        do {
            fprintf(archivo, "%s\n", actual->dato);
            actual = actual->siguiente;
        } while (actual != lista->cabeza);
    }
    
    fprintf(archivo, "===================================\n");
    fclose(archivo);
}
