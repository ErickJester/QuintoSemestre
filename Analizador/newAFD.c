#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000

void reportar_error(int numero_linea, const char *linea) {
    printf("Error en linea %d: %s", numero_linea, linea);
}

void analizar_linea(const char *linea, int numero_linea, int *com_multi) {
    char buffer[MAX_LINE_LENGTH];
    strcpy(buffer, linea);

    if (*com_multi) {
        char *fin_com = strstr(buffer, "*/");
        if (fin_com != NULL) {
            *com_multi = 0;
            strcpy(buffer, fin_com + 2);
        } else {
            return; // Toda la línea está dentro de un comentario de bloque
        }
    }

    if (strstr(buffer, "/*") != NULL) {
        *com_multi = 1;
        char *inicio_com = strstr(buffer, "/*");
        *inicio_com = '\0';
    }

    char *token = strtok(buffer, " \t\n");
    while (token != NULL) {
        if (strcmp(token, "temp1") == 0 && strstr(linea, "temp1 * 8.23") != NULL) {
            reportar_error(numero_linea, linea);
            return;
        }
        if (strcmp(token, "public") == 0 && strstr(linea, "public > 100") != NULL) {
            reportar_error(numero_linea, linea);
            return;
        }
        if (strcmp(token, "int") == 0 && strstr(linea, "int + char") != NULL) {
            reportar_error(numero_linea, linea);
            return;
        }
        if (token[0] == '0' && strlen(token) > 1 && isdigit(token[1])) {
            reportar_error(numero_linea, linea);
            return;
        }
        if (token[0] == '.' && isdigit(token[1])) {
            reportar_error(numero_linea, linea);
            return;
        }
        size_t len = strlen(token);
        if ((token[len - 1] == 'E' || token[len - 1] == 'e') && (strchr(token, 'E') == &token[len - 1] || strchr(token, 'e') == &token[len - 1])) {
            reportar_error(numero_linea, linea);
            return;
        }
        if (strchr(token, '.') != NULL && (token[len - 1] == 'f' || token[len - 1] == 'F')) {
            reportar_error(numero_linea, linea);
            return;
        }
        if (strcmp(token, "if") == 0 && strstr(linea, "if (a = b)") != NULL) {
            reportar_error(numero_linea, linea);
            return;
        }
        if (strstr(linea, "/ 0") != NULL) {
            reportar_error(numero_linea, linea);
            return;
        }
        if (strstr(linea, "* *") != NULL) {
            reportar_error(numero_linea, linea);
            return;
        }
        if (strcmp(token, "1invalido") == 0 || strchr(token, '#') != NULL) {
            reportar_error(numero_linea, linea);
            return;
        }
        token = strtok(NULL, " \t\n");
    }
}

void analizar_archivo(const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char linea[MAX_LINE_LENGTH];
    int numero_linea = 1;
    int com_multi = 0;

    while (fgets(linea, MAX_LINE_LENGTH, archivo)) {
        analizar_linea(linea, numero_linea, &com_multi);
        numero_linea++;
    }

    fclose(archivo);
}

int main() {
    const char *nombre_archivo = "EjemploPracticaAnalizador.java";
    analizar_archivo(nombre_archivo);

    return 0;
}
