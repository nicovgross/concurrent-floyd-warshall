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
    