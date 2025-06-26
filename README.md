# Floyd-Warshall Concorrente

Este projeto tem como objetivo implementar uma versão concorrente do algoritmo Floyd-Warshall para encontrar caminhos mínimos entre cada par de vértices de um grafo. O algoritmo foi testado com diferentes números de threads e grafos de tamanhos variados para compará-los com a versão sequencial e verificar o ganho de desempenho, fazendo uso da biblioteca *pthreads* do C. 

## Funcionalidades

-  Geração de grafos com pesos aleatórios
-  Implementação do algoritmo de Floyd-Warshall com *pthreads*
-  Leitura binária da matriz de distâncias gerada
-  Scripts de análise de desempenho (tempo, aceleração, eficiência)
-  Comparação automática entre execuções com diferentes números de threads


## Compilação e Execução

### Pré-requisitos:
- GCC com suporte a `-pthread`
- Python 3 com `matplotlib`

### Compilação
Execute:
```bash
make
```
Compila o código necessário, guardando os executáveis na pasta ```bin```.

### Geração dos grafos
```bash
./bin/geraGrafo <V> <E> <arquivo_saida>
```
Gera um grafo com ```<V>``` vértices e ```<E>``` arestas, salvando no arquivo de saída.


### Execução do Floyd-Warshall
```bash
./bin/floyd-warshall <nthreads> <arquivo_entrada> <arquivo_saida>
```
Executa o algoritmo com ```<nthreads>``` threads, lendo o grafo de entrada e salvando os resultados da execução no arquivo de saída.

### Execução Automática dos Testes
Após ter rodado o make, na pasta "./scripts", rode:
```bash
chmod +x run_all.sh
./run_all.sh
```
Talvez seja necessário rodar ``sed -i 's/\r$//' run_all.sh`` antes do ``chmod`` em sistemas Unix, tal que:
```bash
sed -i 's/\r$//' run_all.sh
chmod +x run_all.sh
./run_all.sh
```
Executa todos os testes de desempenho automaticamente, com diferentes números de threads, e gera gráficos de comparação, salvando na pasta ```assets```.

## Resultados
Nesse experimento, chegou-se à conclusão que a solução concorrente só foi vantajosa para grafos maiores, por conta do overhead, já que o custo de criar, sincronizar e finalizar as threads é maior que o custo de fazer os cálculos de forma sequencial.
\
\
![Screenshot 2025-06-20 193338](https://github.com/user-attachments/assets/49344bae-c683-4fc8-adb3-e97da6489c85)
![Untitled](https://github.com/user-attachments/assets/d9d51d53-c250-4c39-b9f0-f6b4003127e2)

