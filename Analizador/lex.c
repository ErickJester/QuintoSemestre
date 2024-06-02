#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000

typedef enum { 
    START, INTEGER, ZERO, FLOAT, FLOAT_FRACTION, FLOAT_DOT, EXPONENT, EXPONENT_SIGN, EXPONENT_NUMBER, FLOAT_SUFFIX, ERROR 
} State;

State transition(State current, char input) {
    switch (current) {
        case START:
            if (input == '0') return ZERO;
            else if (isdigit(input)) return INTEGER;
            else if (input == '.') return FLOAT_DOT; // Handle the initial dot case
            else if (input == '+' || input == '-') return START;
            break;
        case ZERO:
            if (input == '.') return FLOAT_FRACTION;
            else if (input == 'E' || input == 'e') return EXPONENT;
            break;
        case INTEGER:
            if (isdigit(input)) return INTEGER;
            else if (input == '.') return FLOAT_FRACTION;
            else if (input == 'E' || input == 'e') return EXPONENT;
            break;
        case FLOAT_DOT:
            if (isdigit(input)) return FLOAT_FRACTION;
            else return ERROR; // Error if there are no digits after the dot
        case FLOAT_FRACTION:
            if (isdigit(input)) return FLOAT_FRACTION;
            else if (input == 'E' || input == 'e') return EXPONENT;
            else if (input == 'f' || input == 'F') return FLOAT_SUFFIX;
            break;
        case EXPONENT:
            if (isdigit(input)) return EXPONENT_NUMBER;
            else if (input == '+' || input == '-') return EXPONENT_SIGN;
            break;
        case EXPONENT_SIGN:
            if (isdigit(input)) return EXPONENT_NUMBER;
            break;
        case EXPONENT_NUMBER:
            if (isdigit(input)) return EXPONENT_NUMBER;
            else if (input == 'f' || input == 'F') return FLOAT_SUFFIX;
            break;
        case FLOAT_SUFFIX:
            return ERROR; // Any further input after the 'f' is an error
        default:
            return ERROR;
    }
    return ERROR;
}

int is_numeric_constant(const char *str) {
    State state = START;
    int i = 0;
    while (str[i] != '\0') {
        state = transition(state, str[i]);
        if (state == ERROR) {
            return 0; // Indicates error
        }
        i++;
    }
    return (state == INTEGER || state == ZERO || state == FLOAT_FRACTION || state == EXPONENT_NUMBER || state == FLOAT_SUFFIX);
}

void analyze_line(const char *line, int line_number) {
    char *token;
    char buffer[MAX_LINE_LENGTH];
    strcpy(buffer, line);

    token = strtok(buffer, " ,;(){}[]<>+-*/%=&|^!~\n\t\r");
    while (token != NULL) {
        if (isdigit(token[0]) || token[0] == '.' || token[0] == '+' || token[0] == '-') {
            if (!is_numeric_constant(token)) {
                printf("Error en línea %d: %s", line_number, line);
                return;
            }
        }
        token = strtok(NULL, " ,;(){}[]<>+-*/%=&|^!~\n\t\r");
    }
}

void analyze_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("No se pudo abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 1;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        analyze_line(line, line_number);
        line_number++;
    }

    fclose(file);
}

int main() {
    const char *filename = "EjemploPracticaAnalizador.java";
    analyze_file(filename);
    return 0;
}
