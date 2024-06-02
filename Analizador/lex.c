#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { q0, q1, q2, q3, q4, q5, q6, q7, C0, C1, C2, C3, C4, ERROR } State;

State transicion(State current, char input) {
    switch (current) {
        case q0:
            if (isdigit(input)) return input == '0' ? q1 : q2;
            if (input == '.') return q3;
            break;
        case q1:
            if (isdigit(input)) return q2;
            if (input == '.') return q3;
            if (input == 'E' || input == 'e') return q5;
            break;
        case q2:
            if (isdigit(input)) return q2;
            if (input == '.') return q3;
            if (input == 'E' || input == 'e') return q5;
            break;
        case q3:
            if (isdigit(input)) return q4;
            break;
        case q4:
            if (isdigit(input)) return q4;
            if (input == 'E' || input == 'e') return q5;
            break;
        case q5:
            if (isdigit(input) || input == '+' || input == '-') return q6;
            break;
        case q6:
            if (isdigit(input)) return q7;
            break;
        case q7:
            if (isdigit(input)) return q7;
            break;
        default:
            break;
    }
    return ERROR;
}

int is_accepting(State state) {
    return state == q2 || state == q4 || state == q7;
}

void analizar(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir");
        exit(EXIT_FAILURE);
    }

    char linea[256];
    int lineaNumero = 1;
    int error_found = 0;
    while (fgets(linea, sizeof(linea), file)) {
        char *token = strtok(linea, " \t\n,;(){}");
        while (token) {
            State state = q0;
            for (int i = 0; i < strlen(token); ++i) {
                state = transicion(state, token[i]);
                if (state == ERROR) break;
            }
            if (!is_accepting(state) && (isdigit(token[0]) || token[0] == '.' || token[0] == '-' || token[0] == '+')) {
                printf("Error en línea %d. Token inválido: %s\n", lineaNumero, token);
                error_found = 1;
            }
            token = strtok(NULL, " \t\n,;(){}");
        }
        ++lineaNumero;
    }

    if (!error_found) {
        printf("No hay errores de análisis léxico en el archivo %s.\n", filename);
    }

    fclose(file);
}

int main() {
    const char *filename = "EjemploPracticaAnalizador.java";
    analizar(filename);
    return 0;
}
