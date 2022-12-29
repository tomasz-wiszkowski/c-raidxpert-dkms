#!/bin/gnuplot
variants = "ext4-raidxpert ext4-mdadm+rcraid ext4-mdadm+nvme"
dirs = "write rewrite read reread randread randwrite bkwdread recrewrite strideread fwrite frewrite fread freread"
graph_titles = "Write ReWrite Read Reread Random_read Random_write Read_Backwards Record_rewrite Stride_read Fwrite Frewrite Fread Freread"
sizevariants = "4 64 256 2048"

variant(n) = word(variants,n)
sizevariant(n) = word(sizevariants,n)
file(dir_, size_) = sprintf("%s/%s.dat", word(dirs,dir_), sizevariant(size_))
outfile(n) = sprintf("%s/%s.ps", word(dirs,n), word(dirs,n))
graph_title(n) = word(graph_titles,n)


exp_file(variant_,dir_,size_) = sprintf("%s/%s", variant(variant_), file(dir_,size_))
exp_variant(a) = sprintf("%s", variant(a))
exp_plot = "\
plot exp_file(1,i,V) using ($1):($2/1000) title exp_variant(1) smooth sbezier with lines ls 1, \
     exp_file(2,i,V) using ($1):($2/1000) title exp_variant(2) smooth sbezier with lines ls 2, \
     exp_file(3,i,V) using ($1):($2/1000) title exp_variant(3) smooth sbezier with lines ls 3, \
"

# set terminal svg noenhanced background rgb '#f0f0ff' size 1920,1080
set terminal wxt noenhanced background rgb '#f0f0ff' size 1920,1080
print "Click mouse to advance to the next graph"

do for [i=1:13:1] {
  unset multiplot
#  set output sprintf("%s.svg", word(dirs, i))
  set grid lc "#20000000"
  set xtics
  set ytics
  set logscale x 2
  set xlabel "File size in 2^n KBytes"
  set ylabel "MBytes/sec"
  set multiplot layout 2,2 rowsfirst
  V=1
  set title sprintf("%s / chunk size %skb", word(dirs, i), sizevariant(V))
  @exp_plot
  V=2
  set title sprintf("%s / chunk size %skb", word(dirs, i), sizevariant(V))
  @exp_plot
  V=3
  set title sprintf("%s / chunk size %skb", word(dirs, i), sizevariant(V))
  @exp_plot
  V=4
  set title sprintf("%s / chunk size %skb", word(dirs, i), sizevariant(V))
  @exp_plot
  pause mouse any
}

