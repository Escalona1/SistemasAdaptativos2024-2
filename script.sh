#!/bin/bash

input_dir="FFMS_all_instances"
# Valor del umbral
threshold=0.75
time=10
files=("100-300" "100-600" "100-800" "200-300" "200-600" "200-800")
output_file="results_greedy_rand"


# Limpia el archivo de resultados si existe


# Ejecuta el programa para cada archivo en el directorio de entrada
for sufix in "${files[@]}"; do
    > $output_file$sufix.txt
    for file in $input_dir/$sufix-00*.txt; do
        ./greedy_grasp -i "$file" -t $time -tunning -th $threshold | awk '{print $1}' >> $output_file$sufix.txt
    done
done
