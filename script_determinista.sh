#!/bin/bash

input_dir="FFMS_all_instances"
# Valor del umbral
threshold=("0.75" "0.8" "0.85")
time=10
files=("100-300" "100-600" "100-800" "200-300" "200-600" "200-800")
output_file="results_determinista/results_determinista"


# Ejecuta el programa para cada archivo en el directorio de entrada
for thr in "${threshold[@]}"; do
    for sufix in "${files[@]}"; do
        > $output_file-$sufix-$thr.txt
        for file in $input_dir/$sufix-*.txt; do
            ./greedy_determinista -i "$file" -th $thr | grep "cardinality of Ps" | awk '{print $NF}' >> $output_file-$sufix-$thr.txt
        done
    done
done
