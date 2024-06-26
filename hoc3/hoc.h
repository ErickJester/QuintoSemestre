#ifndef HOC_H
#define HOC_H

typedef struct Symbol {
    char *name;
    short type;   // VARIABLE, BLTIN, INDEF, VECTOR
    union {
        double val;
        double (*ptr)(double);
        
    } u;
    struct Symbol *next;
} Symbol;

Symbol *install(char *s, int t, double d), *lookup(char *s);
void execerror(char *s, char *t);

#endif // HOC_H
