#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "operaciones.h"
#include "stack.h"

int esOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

int esNumero(char c) {
    return isdigit(c) || c == '.' || c == ',' || (c >= '0' && c <= '9');
}

int esVariable(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int precedencia(char operador) {
    switch (operador) {
        case '^': return 4;
        case '*':
        case '/':
        case '%': return 3;
        case '+':
        case '-': return 2;
        default: return 0;
    }
}

void invertirCadena(char *cadena) {
    int len = strlen(cadena);
    for (int i = 0; i < len / 2; i++) {
        char temp = cadena[i];
        cadena[i] = cadena[len - i - 1];
        cadena[len - i - 1] = temp;
    }
}

void cambiarParentesis(char *cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (cadena[i] == '(') {
            cadena[i] = ')';
        } else if (cadena[i] == ')') {
            cadena[i] = '(';
        }
    }
}

int validarExpresion(const char *expresion) {
    Pila pila;
    inicializarPila(&pila);
    int i = 0;
    int numParentesis = 0;
    char ultimoChar = '\0';
    
    while (expresion[i] != '\0') {
        char c = expresion[i];
        
        // Validar caracteres permitidos
        if (!esNumero(c) && !esOperador(c) && !esVariable(c) && 
            c != '(' && c != ')' && c != '.' && c != ' ') {
            fprintf(stderr, "Error: Caracter no válido '%c'\n", c);
            return 0;
        }
        
        // Validar paréntesis
        if (c == '(') {
            apilar(&pila, "(");
            numParentesis++;
        } else if (c == ')') {
            if (pilaVacia(&pila)) {
                fprintf(stderr, "Error: Paréntesis de cierre sin apertura\n");
                return 0;
            }
            desapilar(&pila);
            numParentesis--;
        }
        
        // Validar operadores consecutivos
        if (esOperador(c) && esOperador(ultimoChar)) {
            fprintf(stderr, "Error: Operadores consecutivos '%c%c'\n", ultimoChar, c);
            return 0;
        }
        
        ultimoChar = c;
        i++;
    }
    
    if (numParentesis != 0) {
        fprintf(stderr, "Error: Paréntesis desbalanceados\n");
        return 0;
    }
    
    return 1;
}

void imprimirEstado(const char *etiqueta, Pila *pila, const char *resultado, int paso) {
    printf("\n=== Paso %d ===\n", paso);
    printf("Acción: %s\n", etiqueta);
    printf("Pila: ");
    
    if (pilaVacia(pila)) {
        printf("(vacía)");
    } else {
        for (int i = 0; i <= pila->cima; i++) {
            printf("%s ", pila->elementos[i]);
        }
    }
    
    printf("\nResultado parcial: %s\n", resultado);
}

void infijaAPostfijaPasoAPaso(const char *infija, char *postfija) {
    Pila pila;
    inicializarPila(&pila);
    int i = 0, j = 0, paso = 1;
    char token[100] = "";
    int tokenIndex = 0;
    
    printf("\n--- Conversión Infija a Postfija (Paso a paso) ---\n");
    
    while (infija[i] != '\0') {
        char c = infija[i];
        
        if (c == ' ') {
            i++;
            continue;
        }
        
        // Manejar números y variables de múltiples caracteres
        if (esNumero(c) || esVariable(c)) {
            token[0] = c;
            token[1] = '\0';
            tokenIndex = 1;
            
            // Leer número/variable completo
            while (esNumero(infija[i+1]) || esVariable(infija[i+1])) {
                token[tokenIndex++] = infija[++i];
            }
            token[tokenIndex] = '\0';
            
            // Agregar al resultado
            if (j > 0) strcat(postfija + j++, " ");
            strcpy(postfija + j, token);
            j += strlen(token);
            
            imprimirEstado("Agregar operando/variable", &pila, postfija, paso++);
        }
        else if (c == '(') {
            apilar(&pila, "(");
            imprimirEstado("Apilar '('", &pila, postfija, paso++);
        }
        else if (c == ')') {
            while (!pilaVacia(&pila) && strcmp(consultarCima(&pila), "(") != 0) {
                if (j > 0) strcat(postfija + j++, " ");
                strcat(postfija + j, desapilar(&pila));
                j += strlen(postfija + j);
                imprimirEstado("Desapilar operador", &pila, postfija, paso++);
            }
            desapilar(&pila); // Remover '('
            imprimirEstado("Remover '('", &pila, postfija, paso++);
        }
        else if (esOperador(c)) {
            char opStr[2] = {c, '\0'};
            while (!pilaVacia(&pila) && 
                   precedencia(consultarCima(&pila)[0]) >= precedencia(c) &&
                   strcmp(consultarCima(&pila), "(") != 0) {
                if (j > 0) strcat(postfija + j++, " ");
                strcat(postfija + j, desapilar(&pila));
                j += strlen(postfija + j);
                imprimirEstado("Desapilar operador por precedencia", &pila, postfija, paso++);
            }
            apilar(&pila, opStr);
            imprimirEstado("Apilar operador", &pila, postfija, paso++);
        }
        
        i++;
    }
    
    while (!pilaVacia(&pila)) {
        if (j > 0) strcat(postfija + j++, " ");
        strcat(postfija + j, desapilar(&pila));
        j += strlen(postfija + j);
        imprimirEstado("Desapilar operadores restantes", &pila, postfija, paso++);
    }
    
    printf("\n--- Conversión completada ---\n");
}

void infijaAPrefijaPasoAPaso(const char *infija, char *prefija) {
    char invertida[MAX], postfija[MAX];
    Pila pila;
    inicializarPila(&pila);
    
    printf("\n--- Conversión Infija a Prefija (Paso a paso) ---\n");
    
    // Paso 1: Invertir expresión
    strcpy(invertida, infija);
    invertirCadena(invertida);
    cambiarParentesis(invertida);
    printf("Paso 1: Expresión invertida: %s\n", invertida);
    
    // Paso 2: Convertir a postfija
    infijaAPostfijaPasoAPaso(invertida, postfija);
    
    // Paso 3: Invertir postfija para obtener prefija
    strcpy(prefija, postfija);
    invertirCadena(prefija);
    printf("Paso 3: Prefija final: %s\n", prefija);
}

double evaluarPostfija(const char *postfija) {
    Pila pila;
    inicializarPila(&pila);
    char token[100];
    int i = 0, tokenIndex = 0;
    
    printf("\n--- Evaluación Postfija ---\n");
    
    while (postfija[i] != '\0') {
        if (postfija[i] == ' ') {
            i++;
            continue;
        }
        
        // Leer token completo
        tokenIndex = 0;
        while (postfija[i] != ' ' && postfija[i] != '\0') {
            token[tokenIndex++] = postfija[i++];
        }
        token[tokenIndex] = '\0';
        
        if (esNumero(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            apilar(&pila, token);
            printf("Apilar número: %s\n", token);
        } else if (esOperador(token[0])) {
            if (pilaVacia(&pila) || pila.cima < 1) {
                fprintf(stderr, "Error: Expresión postfija inválida\n");
                return NAN;
            }
            
            double b = atof(desapilar(&pila));
            double a = atof(desapilar(&pila));
            double resultado;
            
            switch (token[0]) {
                case '+': resultado = a + b; break;
                case '-': resultado = a - b; break;
                case '*': resultado = a * b; break;
                case '/': 
                    if (b == 0) {
                        fprintf(stderr, "Error: División por cero\n");
                        return NAN;
                    }
                    resultado = a / b; 
                    break;
                case '^': resultado = pow(a, b); break;
                case '%': resultado = fmod(a, b); break;
                default: resultado = 0;
            }
            
            char resultadoStr[50];
            sprintf(resultadoStr, "%.10f", resultado);
            apilar(&pila, resultadoStr);
            printf("Operación: %.2f %c %.2f = %.2f\n", a, token[0], b, resultado);
        }
    }
    
    if (pilaVacia(&pila)) {
        fprintf(stderr, "Error: Expresión vacía\n");
        return NAN;
    }
    
    double resultadoFinal = atof(desapilar(&pila));
    printf("Resultado final: %.2f\n", resultadoFinal);
    
    return resultadoFinal;
}

double evaluarPrefija(const char *prefija) {
    Pila pila;
    inicializarPila(&pila);
    char token[100];
    int len = strlen(prefija);
    
    printf("\n--- Evaluación Prefija ---\n");
    
    // Leer de derecha a izquierda
    for (int i = len - 1; i >= 0; i--) {
        if (prefija[i] == ' ') {
            continue;
        }
        
        if (esNumero(prefija[i]) || (prefija[i] == '-' && i > 0 && esNumero(prefija[i-1]))) {
            // Leer número completo
            int start = i;
            while (i >= 0 && (esNumero(prefija[i]) || prefija[i] == '-' || prefija[i] == '.')) {
                i--;
            }
            i++;
            
            // Extraer token
            int tokenIndex = 0;
            for (int j = i; j <= start; j++) {
                token[tokenIndex++] = prefija[j];
            }
            token[tokenIndex] = '\0';
            
            apilar(&pila, token);
            printf("Apilar número: %s\n", token);
        } else if (esOperador(prefija[i])) {
            if (pilaVacia(&pila) || pila.cima < 1) {
                fprintf(stderr, "Error: Expresión prefija inválida\n");
                return NAN;
            }
            
            double a = atof(desapilar(&pila));
            double b = atof(desapilar(&pila));
            double resultado;
            
            switch (prefija[i]) {
                case '+': resultado = a + b; break;
                case '-': resultado = a - b; break;
                case '*': resultado = a * b; break;
                case '/': 
                    if (b == 0) {
                        fprintf(stderr, "Error: División por cero\n");
                        return NAN;
                    }
                    resultado = a / b; 
                    break;
                case '^': resultado = pow(a, b); break;
                case '%': resultado = fmod(a, b); break;
                default: resultado = 0;
            }
            
            char resultadoStr[50];
            sprintf(resultadoStr, "%.10f", resultado);
            apilar(&pila, resultadoStr);
            printf("Operación: %.2f %c %.2f = %.2f\n", a, prefija[i], b, resultado);
        }
    }
    
    if (pilaVacia(&pila)) {
        fprintf(stderr, "Error: Expresión vacía\n");
        return NAN;
    }
    
    double resultadoFinal = atof(desapilar(&pila));
    printf("Resultado final: %.2f\n", resultadoFinal);
    
    return resultadoFinal;
}
