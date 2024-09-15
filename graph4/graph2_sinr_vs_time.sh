#!/bin/bash

# Define input files and output file
input_files=("sinr_fdbet.txt" "sinr_fdmtff.txt" "sinr_pfff.txt" "sinr_rrff.txt")
output_file="graph4_sinr.png"

# Create a temporary file for gnuplot commands
gnuplot_script=$(mktemp)

# Define scheduler names
scheduler_names=("FDBet" "FDMTFF" "PFFF" "RRFF")

# Define line colors
line_colors=("red" "green" "blue" "orange")

# Define the gnuplot commands
cat <<EOF > "$gnuplot_script"
set terminal png size 800,600 #(sets the image format and dimensions)
set output '$output_file'

set xlabel "Time (msec)"
set ylabel "SINR (dBm)"
set title "SINR vs Time at 0 m/s"
set style data linespoints

plot \\
EOF

# Add each file to the plot command with scheduler names and line colors as legends
for ((i=0; i<${#input_files[@]}; i++)); do
    echo -n "\"${input_files[$i]}\" using 1:6 with linespoints title \"SINR ${scheduler_names[$i]} (dBm)\" linecolor rgb '${line_colors[$i]}'"
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

