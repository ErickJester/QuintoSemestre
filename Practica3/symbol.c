#include "hoc.h"
#include "y.tab.h"
#include <string.h>
#include <stdlib.h>



static Symbol *symlist = 0; // tabla de símbolos: lista ligada

Symbol *lookup(char *s) {
    Symbol *sp;
    for (sp = symlist; sp != (Symbol *)0; sp = sp->next) 
        if (strcmp(sp->name, s) == 0) 
            return sp;
    return 0; // NULL, no se encontró
}

Symbol *install(char *s, int t, double d) {
    Symbol *sp;
    sp = (Symbol *) malloc(sizeof(Symbol));
    sp->name = malloc(strlen(s) + 1); // +1 para '\0'
    strcpy(sp->name, s);
    sp->type = t;
    if (t == VAR) {
        sp->u.val = d;
    } else if (t == VECTOR) {
        sp->u.vec = *(Vector *)&d; // Asigna el valor del vector
    }
    sp->next = symlist; // poner al frente de la lista
    symlist = sp;
    return sp;
}

char *emalloc(unsigned n) {
    char *p;
    p = malloc(n);
    if (p == 0)
        execerror("out of memory", (char *) 0);
    return p;
}
