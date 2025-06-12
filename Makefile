# Compilador e flags
CC = gcc
CFLAGS = -Wall -O2

# Pastas
SRC = src
BIN = bin

# Arquivos fonte
FLOYD_SRC = $(SRC)/floyd-warshall.c $(SRC)/leGrafo.c
FLOYD_BIN = $(BIN)/floyd-warshall

GERA_SRC = $(SRC)/geraGrafo.c
GERA_BIN = $(BIN)/geraGrafo

# Alvo padrão
all: $(FLOYD_BIN) $(GERA_BIN)

# Compila floyd-warshall com leGrafo
$(FLOYD_BIN): $(FLOYD_SRC)
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $@ $^

# Compila geraGrafo separadamente
$(GERA_BIN): $(GERA_SRC)
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $@ $^

# Limpeza
clean:
	rm -rf $(BIN)

# Execução (opcional)
run: $(FLOYD_BIN)
	$<

.PHONY: all clean run