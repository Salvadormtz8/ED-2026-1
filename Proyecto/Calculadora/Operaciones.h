#ifndef OPERACIONES_H
#define OPERACIONES_H

#include "stack.h"

int esOperador(char c);
int esNumero(char c);
int esVariable(char c);
int precedencia(char operador);
void invertirCadena(char *cadena);
void cambiarParentesis(char *cadena);
int validarExpresion(const char *expresion);

void infijaAPostfijaPasoAPaso(const char *infija, char *postfija);
void infijaAPrefijaPasoAPaso(const char *infija, char *prefija);
void postfijaAPrefijaPasoAPaso(const char *postfija, char *prefija);
void prefijaAPostfijaPasoAPaso(const char *prefija, char *postfija);
void prefijaAInfijaPasoAPaso(const char *prefija, char *infija);
void postfijaAInfijaPasoAPaso(const char *postfija, char *infija);

double evaluarExpresion(const char *expresion, int esPrefija);
double evaluarPostfija(const char *postfija);
double evaluarPrefija(const char *prefija);

#endif
