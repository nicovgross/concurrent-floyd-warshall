#ifndef LEGRAFO_H
#define LEGRAFO_H

#include <stdio.h>
#include <stdlib.h>

void imprime_matriz_adjacencia(int V, int *adjacencia);
void imprime_dimensoes(int V, int E);
void imprime_grafo(int V, int E, int *adjacencia);
int leGrafo(int **adjacencia, int *V, int *E, char *file);

#endif