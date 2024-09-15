#!/bin/bash

# Gnuplot script
gnuplot_script=$(cat <<END
set terminal pngcairo enhanced font 'Verdana,10'
set output 'graph3_full_5.png'
set xlabel 'Throughput'
set ylabel 'Cumulative probability'
set title 'Cumulative probability vs. Throughput for full buffer at 5.0 m/s'

plot \
  'cumulative_throughput_fdbetff_full_5_23.txt' using 1:2 title 'fdbetff' with linespoints, \
  'cumulative_throughput_fdmtff_full_5_24.txt' using 1:2 title 'fdmtff' with linespoints, \
  'cumulative_throughput_pfff_full_5_25.txt' using 1:2 title 'pfff' with linespoints, \
  'cumulative_throughput_rrff_full_5_26.txt' using 1:2 title 'rrff' with linespoints
END
)

# Execute Gnuplot
echo "$gnuplot_script" | gnuplot

echo "Combined plot saved as 'graph3_full_5.png'"

