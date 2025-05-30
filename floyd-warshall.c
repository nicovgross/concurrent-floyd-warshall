#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "leGrafo.h"

//variáveis globais
int *dist_curr;
int *dist_prev;
int V, E;
int nthreads;

//variáveis de sincronização
pthread_mutex_t mutex;
pthread_cond_t cond;

//copia a matriz 1 para a matriz 2, considerando que elas foram devidamente alocadas
void copia_matriz(int *matriz1, int *matriz2, int dim) {
    for(int i=0; i<(dim*dim); i++) {
        matriz2[i] = matriz1[i];
    }
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

void* Floyd_Warshall(void *arg) {
    int id = (intptr_t) arg;
    
    for(int k=0; k<V; k++) {
        for(int i=id; i<V; i+=nthreads) {
            for(int j=0; j<V; j++) {
                pthread_mutex_lock(&mutex);
                if(dist_prev[i*V + j] > dist_prev[i*V + k] + dist_prev[k*V + j]) {
                    dist_curr[i*V + j] = dist_curr[i*V + k] + dist_curr[k*V + j];
                }
                pthread_mutex_unlock(&mutex);
            }
        }
        barreira(nthreads);
        copia_matriz(dist_curr, dist_prev, V);
    }
}

int main(int argc, char* argv[]) {
    pthread_t *tid;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&cond, NULL);

    if(argc < 3) {
        fprintf(stderr, "Digite: %s <numero de threads> <arquivo de entrada>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]);
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para as threads.\n");
        return 2;
    }

    leGrafo(&dist_curr, &V, &E, argv[2]);
    printf("Matriz de entrada:\n\n");
    imprime_grafo(V, E, dist_curr);
    printf("\n\n");
    dist_prev = (int*) malloc(sizeof(int) * V*V);
    copia_matriz(dist_curr, dist_prev, V);
    
    //Cria as threads
    for(int id=0; id<nthreads; id++) {
        pthread_create(&tid[id], NULL, Floyd_Warshall, (void*)(intptr_t)id);
    }

    for (int id=0; id<nthreads; id++) {
        if (pthread_join(tid[id], NULL)) {
            printf("--ERRO: pthread_join() \n"); 
            exit(-1); 
        } 
    } 

    printf("Matriz de saída:\n\n");
    imprime_grafo(V, E, dist_curr);

    //Espera todas as threads completarem
    free(tid);
    free(dist_curr);
    free(dist_prev); 
    return 0;
}