#!/bin/bash

set -o xtrace

# Criando os arquivos de log
echo "" > /home/lmanrique/mac5742/EP02/mandelbrot_pth.log
echo "" > /home/lmanrique/mac5742/EP02/mandelbrot_omp.log
echo "" > /home/lmanrique/mac5742/EP02/mandelbrot_seq.log

# Rodando 15 vezes para a versão sequencial
/home/lmanrique/perf/linux/tools/perf/perf stat -r 15 /home/lmanrique/mac5742/EP02/mandelbrot_seq >> /home/lmanrique/mac5742/EP02/mandelbrot_seq.log 2>&1


# Rodando 15 vezes para as versoes pthreads e openMP
for i in {0..10}; do 
	/home/lmanrique/perf/linux/tools/perf/perf stat -r 15 /home/lmanrique/mac5742/EP02/mandelbrot_pth $((2**i)) >> /home/lmanrique/mac5742/EP02/mandelbrot_pth.log 2>&1
	/home/lmanrique/perf/linux/tools/perf/perf stat -r 15 /home/lmanrique/mac5742/EP02/mandelbrot_omp $((2**i)) >> /home/lmanrique/mac5742/EP02/mandelbrot_omp.log 2>&1
done
