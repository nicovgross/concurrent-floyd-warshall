#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "leGrafo.h"

//variáveis globais
int *dist_curr; //matriz de distâncias da iteração atual
int *dist_prev; //matriz de distâncias da iteração anterior
int V, E; //Número de vértices e arestas
int nthreads; //número de threads

//variáveis de sincronização
pthread_mutex_t mutex;
pthread_cond_t cond;

void copia_matriz(int *src, int *dest, int n) {
    memcpy(dest, src, n * n * sizeof(int));
}

//funcao barreira
void barreira(int nthreads) {
    static int bloqueadas = 0;
    pthread_mutex_lock(&mutex); //inicio secao critica
    if (bloqueadas == (nthreads-1)) { 
        //ultima thread a chegar na barreira
        pthread_cond_broadcast(&cond);
        bloqueadas=0;
    } else {
        bloqueadas++;
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex); //fim secao critica
}

/*
Algoritmo do Floyd-Warshall
Recebe o id da thread e com base nisso calcula as respectivas linhas da matriz de distância,
sempre consultando a matriz de distâncias da iteração anterior para calcular os valores da matriz atual
*/

void* Floyd_Warshall(void *arg) {
    int id = (intptr_t) arg;
    
    for(int k=0; k<V; k++) {
        for(int i=id; i<V; i+=nthreads) {
            for(int j=0; j<V; j++) {
                if(dist_prev[i*V + j] > dist_prev[i*V + k] + dist_prev[k*V + j]) {
                    dist_curr[i*V + j] = dist_curr[i*V + k] + dist_curr[k*V + j];
                }
            }
        }
        barreira(nthreads);
        copia_matriz(dist_curr, dist_prev, V);
    }
}

int main(int argc, char* argv[]) {
    pthread_t *tid;
    int *adjacencia; //matriz de adjacencia do grafo
    struct timespec inicio, fim;
    double temp_exec, temp_medio;
    FILE *arq;
    size_t ret;

    //inicia as variáveis de sincronização
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&cond, NULL);

    if(argc < 4) {
        fprintf(stderr, "Digite: %s <numero de threads> <arquivo de entrada> <arquivo de saída>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]); //lê o número de threads
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para as threads.\n");
        return 2;
    }

    leGrafo(&adjacencia, &V, &E, argv[2]); //lê o grafo do arquivo de entrada
    dist_curr = (int*) malloc(sizeof(int) * V*V);
    dist_prev = (int*) malloc(sizeof(int) * V*V);

    for(int i=0; i<5; i++) {
        //antes do algoritmo começar as matrizes de distancia vão ser iguais a de adjacência
        copia_matriz(adjacencia, dist_curr, V);
        copia_matriz(adjacencia, dist_prev, V);

        clock_gettime(CLOCK_MONOTONIC, &inicio);
        //Cria as threads
        for(int id=0; id<nthreads; id++) {
            pthread_create(&tid[id], NULL, Floyd_Warshall, (void*)(intptr_t)id);
        }

        //Espera todas as threads completarem
        for (int id=0; id<nthreads; id++) {
            if (pthread_join(tid[id], NULL)) {
                printf("--ERRO: pthread_join() \n"); 
                exit(-1); 
            } 
        }

        clock_gettime(CLOCK_MONOTONIC, &fim);
        temp_exec = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
        temp_medio += temp_exec;
    } 

    temp_medio /= 5; //calcula o tempo de execução médio

    //Escreve todas as informações da execução do programa em um arquivo binário
    arq = fopen(argv[3], "wb");
    if(!arq) { fprintf(stderr, "Erro de abertura do arquivo de saída\n"); return 3; }

    ret = fwrite(&temp_medio, sizeof(double), 1, arq);
    if(!ret) { fprintf(stderr, "Erro na escrita do arquivo de saída\n"); return 4; }
    ret = fwrite(&nthreads, sizeof(int), 1, arq);
    if(!ret) { fprintf(stderr, "Erro na escrita do arquivo de saída\n"); return 4; }
    ret = fwrite(&V, sizeof(int), 1, arq);
    if(!ret) { fprintf(stderr, "Erro na escrita do arquivo de saída\n"); return 4; }
    ret = fwrite(&E, sizeof(int), 1, arq);
    if(!ret) { fprintf(stderr, "Erro na escrita do arquivo de saída\n"); return 4; }
    ret = fwrite(dist_curr, sizeof(int), V*V, arq);
    if(!ret) { fprintf(stderr, "Erro na escrita do arquivo de saída\n"); return 4; }

    //Libera a memória alocada
    free(tid);
    free(dist_curr);
    free(dist_prev); 
    return 0;
}