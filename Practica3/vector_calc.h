#ifndef VECTOR_CALC_H
#define VECTOR_CALC_H

#include "hoc.h"

// Funciones de vector
Vector *creaVector(int n);
void imprimeVector(Vector *a);
Vector *copiaVector(Vector *a);
Vector *sumaVector(Vector *a, Vector *b);
Vector *restaVector(Vector *a, Vector *b);
Vector *escalarVector(double c, Vector *v);
Vector *productoCruz(Vector *a, Vector *b);
double productoPunto(Vector *a, Vector *b);
double vectorMagnitud(Vector *a);

#endif // VECTOR_CALC_H
