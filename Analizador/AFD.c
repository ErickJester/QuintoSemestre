#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 1000

typedef enum { 
    INICIO, C0, CERO, C2, C1, FLOT_PUNTO, C3, SIGNO_EXP, NUM_EXP, C4, C5, C6, COM_MULTI, FIN_COM, ERROR, C7, C8, C9, B, F, P, S, RESERVADA, OPERADOR 
} State;

typedef enum { 
    TIPO_INT, TIPO_FLOAT, TIPO_DOUBLE, TIPO_DESCONOCIDO 
} TipoVar;

// Función de transición
State transicion(State actual, char entrada) {
    switch (actual) {
        case INICIO:
            if (entrada == '0') return CERO;
            else if (isdigit(entrada)) return C0; // Constante entera
            else if (entrada == '.') return FLOT_PUNTO;
            else if (entrada == '+' || entrada == '-') return S; // Signos de valor numérico
            else if (isalpha(entrada) || entrada == '_') return C5; // Identificador válido en Java
            else if (entrada == '/') return C6; // Comentario
            else if (entrada == '<' || entrada == '>' || entrada == '=' || entrada == '!') return C8; // Operadores de comparación y asignación
            else if (entrada == '*' || entrada == '%') return C9; // Operadores aritméticos
            else if (entrada == ';') return F; // Fin de sentencia
            break;
        case CERO:
            if (entrada == '.') return FLOT_PUNTO;
            else if (entrada == 'E' || entrada == 'e') return C3; // Notación exponencial
            break;
        case C0: // Constante entera
            if (isdigit(entrada)) return C0;
            else if (entrada == '.') return FLOT_PUNTO;
            else if (entrada == 'E' || entrada == 'e') return C3; // Notación exponencial
            break;
        case FLOT_PUNTO:
            if (isdigit(entrada)) return C1; // Constante doble
            else return ERROR;
        case C1: // Constante doble
            if (isdigit(entrada)) return C1;
            else if (entrada == 'E' || entrada == 'e') return C3; // Notación exponencial
            else if (entrada == 'f' || entrada == 'F') return C4; // Sufijo flotante
            break;
        case C3: // Notación exponencial
            if (isdigit(entrada)) return NUM_EXP;
            else if (entrada == '+' || entrada == '-') return SIGNO_EXP;
            break;
        case SIGNO_EXP:
            if (isdigit(entrada)) return NUM_EXP;
            break;
        case NUM_EXP:
            if (isdigit(entrada)) return NUM_EXP;
            else if (entrada == 'f' || entrada == 'F') return C4; // Sufijo flotante
            break;
        case C4: // Sufijo flotante
            return C4;
        case C5: // Identificador válido en Java
            if (isalnum(entrada) || entrada == '_') return C5;
            break;
        case C6: // Comentarios
            if (entrada == '*') return COM_MULTI;
            if (entrada == '/') return C6;
            break;
        case COM_MULTI:
            if (entrada == '*') return FIN_COM;
            return COM_MULTI;
        case FIN_COM:
            if (entrada == '/') return INICIO;
            else if (entrada != '*') return COM_MULTI;
            return FIN_COM;
        case C8: // Operadores de comparación y asignación
            if (entrada == '=') return C8;
            return INICIO;
        case C9: // Operadores aritméticos
            return INICIO;
        case S: // Signos de valor numérico
            if (isdigit(entrada)) return C0;
            break;
        case F: // Fin de sentencia
            return INICIO;
        default:
            return ERROR;
    }
    return ERROR;
}

int es_const_num(const char *str, TipoVar tipo) {
    if (str[strlen(str) - 1] == 'f' || str[strlen(str) - 1] == 'F') {
        return 1;
    }

    State estado = INICIO;
    int i = 0;
    while (str[i] != '\0') {
        estado = transicion(estado, str[i]);
        if (estado == ERROR) {
            return 0;
        }
        i++;
    }

    if (tipo == TIPO_INT && strchr(str, '.') != NULL) {
        return 0;
    }

    return (estado == C0 || estado == CERO || estado == C1 || estado == NUM_EXP || estado == C4);
}

void analizar_linea(const char *linea, int numero_linea, int *com_multi) {
    char buffer[MAX];
    strcpy(buffer, linea);

    TipoVar tipo_actual = TIPO_DESCONOCIDO;
    State estado = INICIO;
    int espera_tipo = 0; // Indica si se espera un tipo de variable después de una palabra reservada
    int fin_sentencia = 0;
    int requiere_fin = 0;

    if (*com_multi) {
        char *fin_com = strstr(buffer, "*/");
        if (fin_com != NULL) {
            *com_multi = 0;
            strcpy(buffer, fin_com + 2);
        } else {
            return; // Toda la línea está dentro de un comentario de bloque
        }
    }

    char *token = buffer;
    while (*token != '\0') {
        if (estado == INICIO) {
            if (token[0] == '/' && token[1] == '*') {
                *com_multi = 1;
                token = strstr(token, "*/");
                if (token != NULL) {
                    *com_multi = 0;
                    token += 2;
                } else {
                    break;
                }
            } else if (token[0] == '/' && token[1] == '/') {
                break; // Ignorar el resto de la línea
            } else {
                // Analizar el token como normal
                char *subtoken = strtok(token, " ,;(){}[]<>+-*/%=&|^!~\n\t\r");
                while (subtoken != NULL) {
                    //printf("Analizando token en linea %d: %s\n", numero_linea, subtoken); // Depuración
                    if (strcmp(subtoken, "int") == 0 || strcmp(subtoken, "float") == 0 || strcmp(subtoken, "double") == 0 ||
                        strcmp(subtoken, "public") == 0 || strcmp(subtoken, "private") == 0 || strcmp(subtoken, "class") == 0 || strcmp(subtoken, "void") == 0) {
                        if (espera_tipo && strcmp(subtoken, "class") != 0) {
                            printf("Error en linea %d: %s\n", numero_linea, linea); // Error: Palabra reservada seguida de otra palabra reservada
                            return;
                        }
                        estado = RESERVADA; // Estado para palabras reservadas de Java
                        espera_tipo = 1;
                        //printf("Palabra reservada en linea %d: %s\n", numero_linea, subtoken); // Depuración
                    } else if (subtoken[0] == ';') {
                        estado = F; // Estado para fin de sentencia
                        fin_sentencia = 1;
                        printf("Fin de sentencia en linea %d: %s\n", numero_linea, subtoken); // Depuración
                    } else if (strchr("+-*/%", subtoken[0])) {
                        if (espera_tipo) {
                            printf("Error en linea %d: %s\n", numero_linea, linea); // Error: Operador seguido de palabra reservada
                            return;
                        }
                        estado = OPERADOR; // Estado para operadores aritméticos
                        printf("Operador en linea %d: %s\n", numero_linea, subtoken); // Depuración
                        requiere_fin = 1;
                    } else {
                        if (isdigit(subtoken[0]) || subtoken[0] == '.' || subtoken[0] == '+' || subtoken[0] == '-') {
                            if (espera_tipo) {
                                printf("Error en linea %d: %s\n", numero_linea, linea); // Error: Palabra reservada seguida de un número
                                return;
                            }
                            if (!es_const_num(subtoken, tipo_actual)) {
                                printf("Error en linea %d: %s", numero_linea, linea); // Depuración
                                return;
                            }
                            requiere_fin = 1;
                        } else if (isalpha(subtoken[0]) || subtoken[0] == '_') {
                            estado = INICIO;
                            int i = 0;
                            while (subtoken[i] != '\0') {
                                estado = transicion(estado, subtoken[i]);
                                if (estado == ERROR) {
                                    printf("Error en linea %d: %s\n", numero_linea, linea); // Depuración
                                    return;
                                }
                                i++;
                            }
                            if (estado != C5) { // Identificador válido en Java
                                printf("Error en linea %d: %s\n", numero_linea, linea); // Depuración
                                return;
                            }
                            if (espera_tipo) {
                                espera_tipo = 0; // Después de un identificador, ya no se espera un tipo
                            }
                            requiere_fin = 1;
                        } else {
                            // Manejo de operadores y caracteres inválidos
                            if (strpbrk(subtoken, "#")) {
                                printf("Error en linea %d: %s\n", numero_linea, linea); // Depuración
                                return;
                            } else {
                                printf("Error en linea %d: %s\n", numero_linea, linea); // Depuración
                                return;
                            }
                        }
                        tipo_actual = TIPO_DESCONOCIDO;
                    }
                    subtoken = strtok(NULL, " ,;(){}[]<>+-*/%=&|^!~\n\t\r");
                }
            }
        }
        token++;
    }
    /*
    if (requiere_fin && !fin_sentencia){
        printf("Error en linea %d: Falta punto y coma al final de la sentencia\n", numero_linea);
    }
    */
}

void analizar_archivo(const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char linea[MAX];
    int numero_linea = 1;
    int com_multi = 0;

    while (fgets(linea, MAX, archivo)) {
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
