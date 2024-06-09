%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "vector_calc.h"

void yyerror(const char *s);
int yylex(void);

int in_vector = 0;  // Variable global para controlar el contexto del vector
%}

%union {
    double num;
    Vector *vect;
}

%token <num> NUMBER
%type <vect> vector expVectorial line listnum
%type <num> expEscalar

%left '+' '-'
%left '*'
%left 'x'

%%

input:
    lines
    ;

lines:
    line
    | lines line
    ;

line:
    expVectorial '\n' { imprimeVector($1); printf("Ingrese una expresión (o presione Ctrl+C para salir): \n"); }
    | expEscalar '\n' { printf("%.2f\n", $1); printf("Ingrese una expresión (o presione Ctrl+C para salir): \n"); }
    | '\n' { printf("Ingrese una expresión (o presione Ctrl+C para salir): \n"); }
    ;

expVectorial:
    vector { $$ = $1; }
    | expVectorial '+' expVectorial { $$ = sumaVector($1, $3); }
    | expVectorial '-' expVectorial { $$ = restaVector($1, $3); }
    | expVectorial 'x' expVectorial { $$ = productoCruz($1, $3); }
    | expEscalar '*' expVectorial { $$ = escalarVector($1, $3); }
    | expVectorial '*' expEscalar { $$ = escalarVector($3, $1); }
    | '(' expVectorial ')' { $$ = $2; }
    ;

expEscalar:
    NUMBER { $$ = $1; }
    | '|' expVectorial '|' { $$ = vectorMagnitud($2); }
    | '|' expEscalar '|' { $$ = $2; }
    | expVectorial '*' expVectorial { $$ = productoPunto($1, $3); }
    | '(' expEscalar ')' { $$ = $2; }
    ;

vector:
    '[' listnum ']' { $$ = $2; }
    ;

listnum:
    NUMBER { $$ = creaVector(1); $$->vec[0] = $1; $$->n = 1; }
    | listnum NUMBER {
        Vector *temp = creaVector($1->n + 1);
        for (int i = 0; i < $1->n; i++) {
            temp->vec[i] = $1->vec[i];
        }
        temp->vec[$1->n] = $2;
        temp->n = $1->n + 1;
        $$ = temp;
    }
    ;

%%

int main() {
    
    printf("Ingrese una expresión (o presione Ctrl+C para salir): \n");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int yylex(void) {
    int c;

    while ((c = getchar()) == ' ' || c == '\t') {
        // Skip whitespaces and tabs
    }

    if (c == '[') {
        in_vector = 1;
        return c;
    }

    if (c == ']') {
        in_vector = 0;
        return c;
    }

    if (c == '\n') {
        return c;
    }

    if (c == EOF) return 0;

    if (c == '+' || c == '-' || c == '*' || c == 'x' || c == '|') return c;

    if (isdigit(c) || c == '.') {
        ungetc(c, stdin);
        scanf("%lf", &yylval.num);
        return NUMBER;
    }

    if (in_vector && c == ' ') {
        return yylex();  // Skip spaces inside vector definition
    }

    yyerror("Carácter no reconocido");
    return 0;
}