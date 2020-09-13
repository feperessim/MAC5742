#!/bin/bash

for i in {1..10}; do 
   perf record -o mandelbrot_seq_com_$i.data -g ./mandelbrot_seq_com -0.188 -0.012 0.554 0.754 4096 1; 
done
