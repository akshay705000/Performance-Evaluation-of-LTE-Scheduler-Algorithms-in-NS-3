#!/bin/bash

# Defining the input file and output file.
input_file="rems2.out"
output_file="rem_map.png"

# Creating a temporary file for gnuplot commands.
gnuplot_script=$(mktemp)

# Defining the gnuplot commands.
cat <<EOF > "$gnuplot_script"
set terminal png size 800,600
set output '$output_file'

set title "Radio Environment Map"
set xlabel "X"
set ylabel "Y"
set cblabel "SINR (dB)"
unset key
set colorbox vertical user origin 0.85, 0.2 size 0.05, 0.6

# Defining the range of x and y axes.
set xrange [-1000:2000]
set yrange [-1000:2000]

# Plotting the data points
plot "$input_file" using 1:2:(10*log10(\$4)) with image
EOF

# Executing the gnuplot script.
gnuplot "$gnuplot_script"

# Removing the temporary gnuplot script.
rm "$gnuplot_script"

echo "Plot saved as $output_file"

