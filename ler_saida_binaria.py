import struct
import numpy as np

def ler_arquivo_binario(caminho_arquivo):
    with open(caminho_arquivo, 'rb') as f:
        # Lê o tempo de execução como int (4 bytes) — se o programa for corrigido, mude para 'd' (double)
        temp_exec_bytes = f.read(8)
        temp_exec = struct.unpack('d', temp_exec_bytes)[0]

        #Lê o número de threads
        nthreads_bytes = f.read(4)
        nthreads = struct.unpack('i', nthreads_bytes)[0]

        # Lê número de vértices (int) e arestas (int)
        V_bytes = f.read(4)
        E_bytes = f.read(4)
        V = struct.unpack('i', V_bytes)[0]
        E = struct.unpack('i', E_bytes)[0]

        # Lê a matriz de distâncias (V*V inteiros)
        dist_bytes = f.read(4 * V * V)
        dist_flat = struct.unpack(f'{V*V}i', dist_bytes)
        dist_matrix = np.array(dist_flat).reshape((V, V))

        return temp_exec, nthreads, V, E, dist_matrix
    
    
def ler_matriz_adjacencia_binario(caminho_arquivo):
    with open(caminho_arquivo, 'rb') as f:
        # Lê o número de vértices (V) e arestas (E)
        V = struct.unpack('i', f.read(4))[0]
        E = struct.unpack('i', f.read(4))[0]

        # Lê a matriz de adjacência (V*V inteiros)
        matriz_bytes = f.read(4 * V * V)
        matriz_flat = struct.unpack(f'{V * V}i', matriz_bytes)
        matriz = np.array(matriz_flat, dtype=float).reshape((V, V))

        # Substitui 0 por infinito (exceto na diagonal)
        for i in range(V):
            for j in range(V):
                if i != j and matriz[i][j] == 0:
                    matriz[i][j] = np.inf

    return V, matriz