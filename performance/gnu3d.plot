#!/bin/gnuplot
variants = "ext4-raidxpert ext4-mdadm+rcraid ext4-mdadm+nvme"
dirs = "write rewrite read reread randread randwrite bkwdread recrewrite strideread fwrite frewrite fread freread"
graph_titles = "Write ReWrite Read Reread Random_read Random_write Read_Backwards Record_rewrite Stride_read Fwrite Frewrite Fread Freread"

variant(n) = word(variants,n)
file(n) = sprintf("%s/gnu3d.dat", word(dirs,n))
outfile(n) = sprintf("%s/%s.ps", word(dirs,n), word(dirs,n))
graph_title(n) = word(graph_titles,n)

set title "Iozone performance"
set grid lt 4 lw 1
set surface
set parametric
set xtics
set ytics
set logscale x 2
set logscale y 2
set xlabel "File size in 2^n KBytes"
set ylabel "Record size in 2^n Kbytes"
set zlabel "MBytes/sec"
set style data lines
set dgrid3d 112,112,1
set origin 0,0

exp_file(a,b) = sprintf("%s/%s", variant(a), file(b))
exp_plot = "\
splot exp_file(1,i) using ($1):($2):($3/1000) title variant(1) with lines, \
      exp_file(2,i) using ($1):($2):($3/1000) title variant(2) with lines, \
      exp_file(3,i) using ($1):($2):($3/1000) title variant(3) with lines \
"

do for [i=1:13:1] {
	set title graph_title(i)
	@exp_plot
	pause -1 "Hit return to continue"
}
