set terminal png size 640,480
set output "graph11.png"
set title "Radio Environment Map"
set xlabel "X"
set ylabel "Y"
set cblabel "SINR (dB)"
unset key
set colorbox vertical user origin 0.85, 0.2 size 0.05, 0.6
plot "rems2.out" using 1:2:(10*log10($4)) with image
