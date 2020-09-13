#! /bin/bash

SIZE=48

for ((i=1; i<=$SIZE; i++)); do
	for((j=1; j<=10; j++)); do
    		perf stat ./mandelbrot_pth_com -0.188 -0.012 0.554 0.754 4096 $i >> speedup.log 2>&1
    	done
done
