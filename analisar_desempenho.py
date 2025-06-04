import matplotlib.pyplot as plt
import sys
import os
from ler_saida_binaria import ler_arquivo_binario

temp = []
nthreads = []

Resultados = sys.argv[1]
for resultado in os.listdir(Resultados):
    arquivo = os.path.join(Resultados, resultado)
    temp_exec, threads, V, E, _ = ler_arquivo_binario(arquivo)
    temp.append(temp_exec)
    nthreads.append(threads)

plt.figure(figsize=(10, 6))
plt.bar(nthreads, temp, width=1, edgecolor='black', alpha=0.7)
plt.title(f'V = {V}, E = {E}')
plt.xlabel('Número de threads')
plt.ylabel('Tempo de Execução')
plt.show()

