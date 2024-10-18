#!/bin/bash

input_dir="FFMS_all_instances"
# Archivo para guardar los resultados
output_file="results_greedy_rand100-800.txt"
# Valor del umbral
threshold=0.8

# Limpia el archivo de resultados si existe
> $output_file

# Ejecuta el programa para cada archivo en el directorio de entrada
for file in $input_dir/100-800*.txt; do
    ./r -i "$file" -th $threshold | grep "cardinality of Ps" | awk '{print $NF}' >> $output_file
done