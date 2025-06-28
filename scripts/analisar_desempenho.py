import matplotlib.pyplot as plt
import sys
import os
from ler_saida_binaria import ler_arquivo_binario

def plot_tempo_execucao(threads, tempos, V, E, pasta_saida):
    plt.figure(figsize=(10, 6))
    plt.bar(threads, tempos, width=1, edgecolor='black', alpha=0.7)
    plt.title(f'Tempo de Execução - V = {V}, E = {E}')
    plt.xlabel('Número de threads')
    plt.ylabel('Tempo de Execução (s)')
    plt.xticks(threads)
    plt.savefig(os.path.join(pasta_saida, 'tempo_execucao.png'))
    plt.close()

def plot_aceleracao_eficiencia(threads, tempos, pasta_saida):
    t1 = tempos[threads.index(1)]
    aceleracao = [t1 / t for t in tempos]
    eficiencia = [a / t for a, t in zip(aceleracao, threads)]

    plt.figure(figsize=(10, 6))
    plt.plot(threads, aceleracao, marker='o', label='Aceleração')
    plt.plot(threads, eficiencia, marker='s', label='Eficiência')
    plt.title('Aceleração e Eficiência')
    plt.xlabel('Número de threads')
    plt.ylabel('Valor')
    plt.xticks(threads)
    plt.grid(True)
    plt.legend()

    for t in [2, 4, 8]:
        if t in threads:
            idx = threads.index(t)
            plt.text(t, aceleracao[idx] + 0.05, f"{aceleracao[idx]:.2f}", ha='center', color='blue')
            plt.text(t, eficiencia[idx] - 0.07, f"{eficiencia[idx]:.2f}", ha='center', color='green')

    plt.savefig(os.path.join(pasta_saida, 'aceleracao_eficiencia.png'))
    plt.close()

def processar_resultados(pasta_resultados, pasta_assets):
    for subpasta in os.listdir(pasta_resultados):
        caminho_subpasta = os.path.join(pasta_resultados, subpasta)
        if not os.path.isdir(caminho_subpasta):
            continue

        tempos = []
        threads = []
        V = E = None

        for arquivo in sorted(os.listdir(caminho_subpasta)):
            caminho_arquivo = os.path.join(caminho_subpasta, arquivo)
            temp_exec, nthread, V, E, _ = ler_arquivo_binario(caminho_arquivo)
            tempos.append(temp_exec)
            threads.append(nthread)

        if 1 not in threads:
            print(f"[!] Subpasta '{subpasta}' ignorada: não possui execução com 1 thread.")
            continue

        # Ordena por número de threads
        threads, tempos = zip(*sorted(zip(threads, tempos)))
        threads, tempos = list(threads), list(tempos)

        pasta_saida = os.path.join(pasta_assets, subpasta)
        os.makedirs(pasta_saida, exist_ok=True)

        plot_tempo_execucao(threads, tempos, V, E, pasta_saida)
        plot_aceleracao_eficiencia(threads, tempos, pasta_saida)
        print(f"[✓] Plots salvos em {pasta_saida}")

# Caminhos
PASTA_RESULTADOS = 'data/resultados'
PASTA_ASSETS = 'assets'

# Executa
processar_resultados(PASTA_RESULTADOS, PASTA_ASSETS)