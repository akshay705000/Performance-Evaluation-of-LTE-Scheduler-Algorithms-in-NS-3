#!/bin/bash

# Input files containing scheduler names and average throughputs
input_file_half_0="average_throughputs_half_0.txt"
input_file_half_5="average_throughputs_half_5.txt"

# Output files for the plots (PNG format)
output_file_half_0="bar_chart_half_0.png"
output_file_half_5="bar_chart_half_5.png"

# Gnuplot script to generate the bar charts
gnuplot <<EOF
set terminal pngcairo enhanced font 'Verdana,10'

# First plot for average_throughputs_full_0.txt
set output '${output_file_half_0}'
set title 'Average Throughputs for half 0'
set xlabel 'Scheduler'
set ylabel 'Average Throughput'
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.8
set xtics rotate by -45
set ytics format "%.2e"
plot '${input_file_half_0}' using 2:xtic(1) title ''

# Second plot for average_throughputs_full_5.txt
set output '${output_file_half_5}'
set title 'Average Throughputs for half 5'
plot '${input_file_half_5}' using 2:xtic(1) title ''
EOF

echo "Bar charts generated as '${output_file_half_0}' and '${output_file_half_5}'"

