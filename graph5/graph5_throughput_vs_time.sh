#!/bin/bash

# Define input files and output file
input_files=("throughput_fdbet.txt" "throughput_fdmtff.txt" "throughput_pfff.txt" "throughput_rrff.txt")
output_file="graph5_throughput.png"

# Create a temporary file for gnuplot commands
gnuplot_script=$(mktemp)

# Define scheduler names
scheduler_names=("fdbet" "fdmtff" "pfff" "rrff")

# Define the gnuplot commands
cat <<EOF > "$gnuplot_script"
set terminal pngcairo size 800,600 enhanced font 'Verdana,12'
set output '$output_file'

set title "Throughput vs Time at 5 m/s"
set xlabel "Time (msec)"
set ylabel "SINR"
set key outside right

plot \\
EOF

# Add each file to the plot command with scheduler names as legends
for ((i=0; i<${#input_files[@]}; i++)); do
    echo -n "\"${input_files[$i]}\" using 1:2 with lines title \"${scheduler_names[$i]}\""
    if [ $i -lt $((${#input_files[@]}-1)) ]; then
        echo -n ", \\"
    fi
    echo ""
done >> "$gnuplot_script"

# Execute the gnuplot script
gnuplot "$gnuplot_script"

# Remove the temporary gnuplot script
rm "$gnuplot_script"

echo "Plot saved as $output_file"

