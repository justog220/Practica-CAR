#!/bin/bash
ocurrencias=$(grep -o "A" data/*fasta | wc -l)
echo "El número de ocurrencias de A es: $ocurrencias"

ocurrencias=$(grep -o "T" data/*fasta | wc -l)
echo "El número de ocurrencias de T es: $ocurrencias"

ocurrencias=$(grep -o "G" data/*fasta | wc -l)
echo "El número de ocurrencias de G es: $ocurrencias"

ocurrencias=$(grep -o "C" data/*fasta | wc -l)
echo "El número de ocurrencias de C es: $ocurrencias"