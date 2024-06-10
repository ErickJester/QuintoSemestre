%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "hoc.h"
#include "vector_calc.h"

void yyerror(const char *s);
int yylex(void);

int in_vector = 0;  // Variable global para controlar el contexto del vector
%}

%union {
    double num;
    Symbol *sym;
    Vector *vect;
}

%token <num> NUMBER
%token <sym> VAR BLTIN INDEF
%type <vect> vector expVectorial line listnum
%type <num> expEscalar
%type <sym> asgn

%left '+' '-'
%left '*'
%left 'x'
%right '='

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
    | asgn '\n' { printf("Variable asignada\n"); printf("Ingrese una expresión (o presione Ctrl+C para salir): \n"); }
    | '\n' { printf("Ingrese una expresión (o presione Ctrl+C para salir): \n"); }
    ;

asgn:
    VAR '=' vector { printf("Asignando: %s\n", $1->name); $$ = $1; $1->u.vec = *$3; $1->type = VECTOR; }
    ;

expVectorial:
    vector { $$ = $1; }
    | expVectorial '+' expVectorial { $$ = sumaVector($1, $3); }
    | expVectorial '-' expVectorial { $$ = restaVector($1, $3); }
    | expVectorial 'x' expVectorial { $$ = productoCruz($1, $3); }
    | expEscalar '*' expVectorial { $$ = escalarVector($1, $3); }
    | expVectorial '*' expEscalar { $$ = escalarVector($3, $1); }
    | VAR { if ($1->type != VECTOR) execerror("variable no es un vector", $1->name); $$ = &$1->u.vec; }
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
    printf("\n\nIngrese una expresión (o presione Ctrl+C para salir): \n");
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
    //printf("Leyendo caracter: %c\n", c);

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

    if (c == '+' || c == '-' || c == '*' || c == 'x' || c == '|' || c == '=') return c;

    if (isdigit(c) || c == '.') {
        ungetc(c, stdin);
        scanf("%lf", &yylval.num);
        return NUMBER;
    }

    if (isalpha(c)) {
        Symbol *s;
        char sbuf[10000], *p = sbuf;
        do {
            *p++ = c;
        } while ((c = getchar()) != EOF && isalnum(c));
        ungetc(c, stdin);
        *p = '\0';
        if ((s = lookup(sbuf)) == (Symbol *)NULL)
            s = install(sbuf, INDEF, 0.0);
        yylval.sym = s;
        return VAR;
    }

    if (in_vector && c == ' ') {
        return yylex();  // Skip spaces inside vector definition
    }

    yyerror("Carácter no reconocido");
    return 0;
}
