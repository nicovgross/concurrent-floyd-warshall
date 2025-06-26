#!/bin/bash

# Get directory of this script and resolve project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"

# Paths (relative to project root)
GRAPH_DIR="$ROOT_DIR/data/grafos_testes"
RESULTS_DIR="$ROOT_DIR/data/resultados"
EXECUTABLE="$ROOT_DIR/bin/floyd-warshall"

# Thread configurations
THREADS=(1 2 4 8)

# Temporary working directory
TMP_DIR="./tmp_graphs"
mkdir -p "$TMP_DIR"

# Find all .bin and .bin.gz files
find "$GRAPH_DIR" \( -name "*.bin" -o -name "*.bin.gz" \) | while read -r input_file; do
    base_filename=$(basename -- "$input_file")
    
    if [[ "$base_filename" == *.gz ]]; then
        # Remove .gz to get name_no_ext
        name_no_ext="${base_filename%.bin.gz}"
        decompressed_file="$TMP_DIR/$name_no_ext.bin"
        
        echo "Decompressing $base_filename..."
        gunzip -c "$input_file" > "$decompressed_file"
        used_file="$decompressed_file"
    else
        name_no_ext="${base_filename%.bin}"
        used_file="$input_file"
    fi

    # Ensure result directory exists
    mkdir -p "$RESULTS_DIR/$name_no_ext"

    for t in "${THREADS[@]}"; do
        output_file="$RESULTS_DIR/$name_no_ext/resultado-${t}thread.bin"
        echo "Running $name_no_ext with $t threads..."
        $EXECUTABLE "$t" "$used_file" "$output_file"
    done
done

# Clean up
rm -rf "$TMP_DIR"