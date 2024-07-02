#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 256

typedef struct {
    char symbols[10][MAX_BUFFER];
    char states[10][MAX_BUFFER];
    char accepting_states[10][MAX_BUFFER];
    char equations[50][MAX_BUFFER];
    int num_symbols;
    int num_states;
    int num_accepting_states;
    int num_equations;
} AFD;

// Función auxiliar para simplificar la expresión eliminando duplicados
void simplificar_expresion(char *expresion) {
    char temp[MAX_BUFFER];
    char *tokens[MAX_BUFFER];
    int token_count = 0;
    int i, j;

    strcpy(temp, expresion);
    char *token = strtok(temp, "+");
    while (token != NULL) {
        int es_duplicado = 0;
        for (i = 0; i < token_count; i++) {
            if (strcmp(tokens[i], token) == 0) {
                es_duplicado = 1;
                break;
            }
        }
        if (!es_duplicado) {
            tokens[token_count++] = token;
        }
        token = strtok(NULL, "+");
    }

    if (token_count > 0) {
        strcpy(expresion, tokens[0]);
    } else {
        expresion[0] = '\0';
    }
    for (i = 1; i < token_count; i++) {
        strcat(expresion, "+");
        strcat(expresion, tokens[i]);
    }

    // Depuración
    printf("Expresión simplificada: %s\n", expresion);
}



void limpiar_y_validar(char *expresion) {
    char temp[MAX_BUFFER];
    int i, j = 0;

    for (i = 0; i < strlen(expresion); i++) {
        if (expresion[i] != ' ') {
            temp[j++] = expresion[i];
        }
    }
    temp[j] = '\0';
    strcpy(expresion, temp);

    int balance = 0;
    for (i = 0; i < strlen(expresion); i++) {
        if (expresion[i] == '(') {
            balance++;
        } else if (expresion[i] == ')') {
            balance--;
        }
        if (balance < 0) {
            printf("Error: parentesis no balanceados.\n");
            exit(EXIT_FAILURE);
        }
    }
    if (balance != 0) {
        printf("Error: parentesis no balanceados.\n");
        exit(EXIT_FAILURE);
    }
}


// Función para reemplazar todas las ocurrencias de un substring en una string   
void replace_all(char *str, const char *old, const char *new_str) {
    char buffer[4096];  // Aumentar el tamaño del buffer si es necesario
    char *pos;
    int old_len = strlen(old);
    int new_len = strlen(new_str);

    buffer[0] = '\0';

    while ((pos = strstr(str, old)) != NULL) {
        strncat(buffer, str, pos - str);
        strcat(buffer, new_str);
        str = pos + old_len;
    }
    strcat(buffer, str);
    strcpy(str, buffer);

    // Depuración
    printf("Reemplazo de '%s' por '%s': %s\n", old, new_str, str);
}


// Función auxiliar para agrupar expresiones lógicamente
void agrupar_expresion(char *nueva_expresion, char *resultado_actual, char *nuevo_resultado) {
    if (strlen(resultado_actual) == 0) {
        strcpy(nuevo_resultado, nueva_expresion);
    } else {
        snprintf(nuevo_resultado, MAX_BUFFER, "(%s)+(%s)", nueva_expresion, resultado_actual);
    }

    // Depuración
    printf("Expresión agrupada: %s\n", nuevo_resultado);
}


// Función principal para aplicar el Teorema de Arden
void aplicar_teorema_arden(AFD *afd, char *expresion_regular) {
    char resultado[MAX_BUFFER] = "";

    for (int i = 0; i < afd->num_equations; i++) {
        char temp[MAX_BUFFER];
        strcpy(temp, afd->equations[i]);

        char *li = strtok(temp, "=");
        char *ld = strtok(NULL, "=");

        printf("Procesando ecuacion %d: %s = %s\n", i, li, ld);

        char simp_ld[MAX_BUFFER];
        int j = 0;
        for (int k = 0; k < strlen(ld); k++) {
            if (ld[k] != ' ') {
                simp_ld[j++] = ld[k];
            }
        }
        simp_ld[j] = '\0';

        printf("Lado derecho simplificado: %s\n", simp_ld);

        for (int k = 0; k < afd->num_states; k++) {
            char var[3];
            sprintf(var, "X%d", k);
            replace_all(simp_ld, var, resultado);
        }

        printf("Lado derecho despues de reemplazar variables: %s\n", simp_ld);

        limpiar_y_validar(simp_ld);

        printf("Lado derecho despues de limpiar y validar: %s\n", simp_ld);

        char nuevo_resultado[MAX_BUFFER];
        agrupar_expresion(simp_ld, resultado, nuevo_resultado);
        strcpy(resultado, nuevo_resultado);

        printf("Resultado acumulado: %s\n", resultado);
    }

    simplificar_expresion(resultado);

    printf("Resultado final simplificado: %s\n", resultado);

    strcpy(expresion_regular, resultado);
}

// Función para leer el AFD desde un archivo
void leer_afd(const char *filename, AFD *afd) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER];
    // Leer símbolos del alfabeto
    fgets(buffer, MAX_BUFFER, file);
    char *token = strtok(buffer, ", ");
    afd->num_symbols = 0;
    while (token) {
        strcpy(afd->symbols[afd->num_symbols++], token);
        token = strtok(NULL, ", ");
    }

    // Leer estados
    fgets(buffer, MAX_BUFFER, file);
    token = strtok(buffer, ", ");
    afd->num_states = 0;
    while (token) {
        strcpy(afd->states[afd->num_states++], token);
        token = strtok(NULL, ", ");
    }

    // Leer estados de aceptación
    fgets(buffer, MAX_BUFFER, file);
    token = strtok(buffer, ", ");
    afd->num_accepting_states = 0;
    while (token) {
        strcpy(afd->accepting_states[afd->num_accepting_states++], token);
        token = strtok(NULL, ", ");
    }

    // Leer ecuaciones
    afd->num_equations = 0;
    while (fgets(buffer, MAX_BUFFER, file)) {
        strcpy(afd->equations[afd->num_equations++], buffer);
    }

    fclose(file);
}

int main() {
    AFD afd;
    char expresion_regular[MAX_BUFFER];

    // Leer el AFD desde un archivo (asumiendo la implementación de leer_afd)
    leer_afd("AFD.txt", &afd);

    aplicar_teorema_arden(&afd, expresion_regular);
    printf("\nL(AFD) = %s\n", expresion_regular);

    return 0;
}
