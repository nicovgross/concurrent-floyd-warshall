import numpy as np
from scipy.sparse.csgraph import floyd_warshall
from ler_saida_binaria import ler_matriz_adjacencia_binario, ler_arquivo_binario
import sys

def main():
    if len(sys.argv) != 3:
        print("Uso: python corretude.py <arquivo_adjacencia.bin> <arquivo_distancias.bin>")
        sys.exit(1)

    caminho_adj = sys.argv[1]
    caminho_dist = sys.argv[2]

    V_adj, matriz_adj = ler_matriz_adjacencia_binario(caminho_adj)
    tempo_exec, nthreads, V_dist, E_dist, matriz_usuario_dist = ler_arquivo_binario(caminho_dist)
    matriz_usuario_dist = np.where(matriz_usuario_dist >= 1e9, np.inf, matriz_usuario_dist) # Substitui valores 1e^9 e acime por infinito

    if V_adj != V_dist:
        print("Erro: número de vértices diferentes entre os arquivos.")
        sys.exit(1)

    matriz_adj[matriz_adj >= 1e9] = np.inf
    matriz_scipy = floyd_warshall(matriz_adj, directed=True, unweighted=False, return_predecessors=False)
    
    # Converte valores finitos para inteiros
    matriz_scipy = np.where(np.isfinite(matriz_scipy), matriz_scipy.astype(int), np.inf)
    # Cria máscara de posições válidas (não inf em ambas as matrizes)
    mask = np.isfinite(matriz_scipy) & np.isfinite(matriz_usuario_dist)

    # Cria matriz de diferença só com valores válidos
    diferenca = np.zeros_like(matriz_scipy)
    diferenca[mask] = matriz_scipy[mask] - matriz_usuario_dist[mask]
    frob = np.linalg.norm(diferenca, 'fro')

    print(f"Norma de Frobenius da diferença: {frob**2:.8f}")
    print("Máxima diferença absoluta: ", np.max(np.abs(diferenca[mask])))

if __name__ == "__main__":
    main()