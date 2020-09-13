#!/bin/bash

set -o xtrace

# Criando os arquivos de log
echo "" > /home/lmanrique/mac5742/EP02/mandelbrot_mpi_single.log
echo "" > /home/lmanrique/mac5742/EP02/mandelbrot_mpi_omp_single.log

# Rodando 15 vezes para as versoes pthreads e openMP
for i in {1..16}; do 
	/home/lmanrique/perf/linux/tools/perf/perf stat -r 15 mpirun -n $((2*i)) /home/lmanrique/mac5742/EP02/mandelbrot_mpi >> /home/lmanrique/mac5742/EP02/mandelbrot_mpi_single.log 2>&1
		for j in {0..6}; do
			/home/lmanrique/perf/linux/tools/perf/perf stat -r 15 mpirun -n $((2*i)) /home/lmanrique/mac5742/EP02/mandelbrot_mpi_omp $((2**j)) >> /home/lmanrique/mac5742/EP02/mandelbrot_mpi_omp_single.log 2>&1
		done
done
