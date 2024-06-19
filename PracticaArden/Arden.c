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

    // Tokenizar la expresion por el delimitador '+'
    strcpy(temp, expresion);
    char *token = strtok(temp, "+");
    while (token != NULL) {
        tokens[token_count++] = token;
        token = strtok(NULL, "+");
    }

    // Eliminar duplicados
    for (i = 0; i < token_count; i++) {
        for (j = i + 1; j < token_count; j++) {
            if (strcmp(tokens[i], tokens[j]) == 0) {
                int k;
                for (k = j; k < token_count - 1; k++) {
                    tokens[k] = tokens[k + 1];
                }
                token_count--;
                j--;  // Volver a comprobar la nueva posicion
            }
        }
    }

    // Reconstruir la expresion sin duplicados
    strcpy(expresion, tokens[0]);
    for (i = 1; i < token_count; i++) {
        strcat(expresion, "+");
        strcat(expresion, tokens[i]);
    }
}

void limpiar_y_validar(char *expresion) {
    char temp[MAX_BUFFER];
    int i, j = 0;

    // Eliminar espacios innecesarios
    for (i = 0; i < strlen(expresion); i++) {
        if (expresion[i] != ' ') {
            temp[j++] = expresion[i];
        }
    }
    temp[j] = '\0';
    strcpy(expresion, temp);

    // Validar parentesis balanceados (opcional, pero recomendado)
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
    char buffer[1024];
    char *pos;
    int old_len = strlen(old);
    int new_len = strlen(new_str);

    buffer[0] = '\0';

    while ((pos = strstr(str, old)) != NULL) {
        // Copiar la parte de la cadena antes del substring 'old'
        strncat(buffer, str, pos - str);
        // Anadir el substring 'new_str'
        strcat(buffer, new_str);
        // Mover el puntero de 'str' mas alla del 'old' reemplazado
        str = pos + old_len;
    }
    // Anadir la parte restante de 'str'
    strcat(buffer, str);
    // Copiar el contenido del buffer de vuelta a 'str'
    strcpy(str, buffer);
}


// Función auxiliar para agrupar expresiones lógicamente
void agrupar_expresion(char *nueva_expresion, char *resultado_actual, char *nuevo_resultado) {
    if (strlen(resultado_actual) == 0) {
        strcpy(nuevo_resultado, nueva_expresion);
    } else {
        snprintf(nuevo_resultado, MAX_BUFFER, "(%s) + (%s)", nueva_expresion, resultado_actual);
    }
}

// Función principal para aplicar el Teorema de Arden
void aplicar_teorema_arden(AFD *afd, char *expresion_regular) {
    char resultado[MAX_BUFFER] = "";  // Inicialmente, la expresion regular es vacia
    
    // Iterar sobre las ecuaciones en orden
    for (int i = 0; i < afd->num_equations; i++) {
        char temp[MAX_BUFFER];
        strcpy(temp, afd->equations[i]);

        // Dividir la ecuacion en lado izquierdo (li) y lado derecho (ld)
        char *li = strtok(temp, "=");  // Lado izquierdo de la ecuacion
        char *ld = strtok(NULL, "=");  // Lado derecho de la ecuacion

        printf("Procesando ecuacion %d: %s = %s\n", i, li, ld);

        // Simplificar el lado derecho eliminando espacios en blanco
        char simp_ld[MAX_BUFFER];
        int j = 0;
        for (int k = 0; k < strlen(ld); k++) {
            if (ld[k] != ' ') {
                simp_ld[j++] = ld[k];
            }
        }
        simp_ld[j] = '\0';

        printf("Lado derecho simplificado: %s\n", simp_ld);

        // Reemplazar todas las variables Xn en el lado derecho con el resultado actual
        for (int k = 0; k < afd->num_states; k++) {
            char var[3];
            sprintf(var, "X%d", k);
            replace_all(simp_ld, var, resultado);
        }

        printf("Lado derecho despues de reemplazar variables: %s\n", simp_ld);

        // Limpiar y validar la expresion simplificada
        limpiar_y_validar(simp_ld);

        printf("Lado derecho despues de limpiar y validar: %s\n", simp_ld);

        // Actualizar el resultado acumulado usando la funcion auxiliar de agrupacion
        char nuevo_resultado[MAX_BUFFER];
        agrupar_expresion(simp_ld, resultado, nuevo_resultado);
        strcpy(resultado, nuevo_resultado);

        printf("Resultado acumulado: %s\n", resultado);
    }

    // Simplificar la expresion final eliminando duplicados
    simplificar_expresion(resultado);

    printf("Resultado final simplificado: %s\n", resultado);

    // Copiar el resultado final a la expresion regular
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
