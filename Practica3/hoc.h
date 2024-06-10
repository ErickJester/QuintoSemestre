#ifndef HOC_H
#define HOC_H

typedef struct Vector {
    int n;
    double *vec;
} Vector;

typedef struct Symbol {
    char *name;
    short type;   // VAR, BLTIN, INDEF, VECTOR
    union {
        double val;
        double (*ptr)(double);
        Vector vec;
    } u;
    struct Symbol *next;
} Symbol;
#define VECTOR 4

Symbol *install(char *s, int t, double d), *lookup(char *s);
void execerror(char *s, char *t);

#endif // HOC_H
