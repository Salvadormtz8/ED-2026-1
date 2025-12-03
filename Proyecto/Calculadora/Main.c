#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "dlist.h"
#include "operaciones.h"

//Laureano Sanches Yoshua Barush
//Martinez Galvan Jesus Salvador

void mostrarAyuda() {
    printf("\n=== CALCULADORA ===\n");
    printf("Uso: ./calc [OPCIONES] [EXPRESIÓN]\n\n");
    printf("Opciones:\n");
    printf("  -h, --help        Mostrar esta ayuda\n");
    printf("  -p, --postfija    Convertir a notación postfija\n");
    printf("  -r, --prefija     Convertir a notación prefija\n");
    printf("  -e, --evaluar     Evaluar expresión\n");
    printf("  -s, --pasos       Mostrar pasos detallados\n");
    printf("  -g, --guardar     Guardar resultado en archivo\n\n");
    
    printf("Ejemplos:\n");
    printf("  ./calc \"(a+b)^c\"\n");
    printf("  ./calc -p \"3+4*5\"\n");
    printf("  ./calc -e \"3.14*2^2\"\n");
    printf("  ./calc -r \"(x+y)*z\" -s\n");
    
    printf("\nOperadores soportados:\n");
    printf("  +  -  *  /  ^  %%  ( )\n");
    
    printf("\nCaracterísticas:\n");
    printf("  • Números grandes y decimales\n");
    printf("  • Variables (a-z, A-Z)\n");
    printf("  • Historial de operaciones\n");
    printf("  • Guardado en archivo\n");
    printf("  • Conversiones entre notaciones\n");
    printf("  • Validación de expresiones\n");
}

void guardarResultado(const char *expresion, const char *resultado, 
                     const char *tipo, ListaCircular *historial) {
    char registro[500];
    sprintf(registro, "[%s] %s = %s", tipo, expresion, resultado);
    insertarFinal(historial, registro);
    guardarEnArchivo(historial, "historial_calc.txt");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: Se requiere al menos un argumento\n");
        printf("Use: ./calc -h para ayuda\n");
        return 1;
    }
    
    // Mostrar ayuda si se solicita
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        mostrarAyuda();
        return 0;
    }
    
    ListaCircular *historial = crearLista();
    char expresion[500] = "";
    char resultado[500] = "";
    int mostrarPasos = 0;
    int evaluar = 0;
    int convertirPrefija = 0;
    int convertirPostfija = 0;
    int guardar = 0;
    
    // Procesar argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--pasos") == 0) {
            mostrarPasos = 1;
        } else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--evaluar") == 0) {
            evaluar = 1;
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--postfija") == 0) {
            convertirPostfija = 1;
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--prefija") == 0) {
            convertirPrefija = 1;
        } else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--guardar") == 0) {
            guardar = 1;
        } else if (argv[i][0] != '-') {
            strcpy(expresion, argv[i]);
        }
    }
    
    if (strlen(expresion) == 0) {
        printf("Ingrese la expresión: ");
        fgets(expresion, sizeof(expresion), stdin);
        expresion[strcspn(expresion, "\n")] = '\0';
    }
    
    // Validar expresión
    if (!validarExpresion(expresion)) {
        fprintf(stderr, "Expresión inválida\n");
        destruirLista(historial);
        return 1;
    }
    
    printf("\n=== Expresión ingresada: %s ===\n", expresion);
    
    // Procesar según opciones
    if (convertirPostfija) {
        char postfija[500] = "";
        infijaAPostfijaPasoAPaso(expresion, postfija);
        printf("\n Notación Postfija: %s\n", postfija);
        
        if (evaluar) {
            double resultadoEval = evaluarPostfija(postfija);
            if (!isnan(resultadoEval)) {
                printf("\n Resultado numérico: %.10f\n", resultadoEval);
                sprintf(resultado, "%.10f", resultadoEval);
            }
        }
        
        if (guardar) {
            guardarResultado(expresion, postfija, "POSTFIJA", historial);
        }
    }
    
    if (convertirPrefija) {
        char prefija[500] = "";
        infijaAPrefijaPasoAPaso(expresion, prefija);
        printf("\n Notación Prefija: %s\n", prefija);
        
        if (evaluar) {
            double resultadoEval = evaluarPrefija(prefija);
            if (!isnan(resultadoEval)) {
                printf("\n Resultado numérico: %.10f\n", resultadoEval);
                sprintf(resultado, "%.10f", resultadoEval);
            }
        }
        
        if (guardar) {
            guardarResultado(expresion, prefija, "PREFIJA", historial);
        }
    }
    
    // Si no se especificó conversión, hacer ambas
    if (!convertirPostfija && !convertirPrefija) {
        char postfija[500] = "", prefija[500] = "";
        
        printf("\n--- Conversión a Postfija ---\n");
        infijaAPostfijaPasoAPaso(expresion, postfija);
        printf("\n Postfija: %s\n", postfija);
        
        printf("\n--- Conversión a Prefija ---\n");
        infijaAPrefijaPasoAPaso(expresion, prefija);
        printf("\n Prefija: %s\n", prefija);
        
        if (evaluar) {
            printf("\n--- Evaluación ---\n");
            double resultadoPost = evaluarPostfija(postfija);
            double resultadoPre = evaluarPrefija(prefija);
            
            if (!isnan(resultadoPost) && !isnan(resultadoPre)) {
                printf("\n Resultado desde postfija: %.10f\n", resultadoPost);
                printf(" Resultado desde prefija: %.10f\n", resultadoPre);
                
                if (fabs(resultadoPost - resultadoPre) < 0.0001) {
                    printf("\n Los resultados coinciden!\n");
                }
                
                sprintf(resultado, "Postfija=%.10f, Prefija=%.10f", resultadoPost, resultadoPre);
            }
        }
        
        if (guardar) {
            char registro[1000];
            sprintf(registro, "INFIJA: %s | POSTFIJA: %s | PREFIJA: %s | RESULTADO: %s", 
                   expresion, postfija, prefija, resultado);
            insertarFinal(historial, registro);
            guardarEnArchivo(historial, "historial_calc.txt");
        }
    }
    
    // Mostrar historial si hay elementos
    if (historial->tamano > 0) {
        printf("\n Historial de esta sesión:\n");
        mostrarLista(historial);
    }
    
    destruirLista(historial);
    return 0;
}
