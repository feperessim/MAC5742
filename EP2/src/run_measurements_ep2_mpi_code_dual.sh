#!/bin/bash

set -o xtrace

# Criando os arquivos de log
echo "" > /home/lmanrique/mac5742/EP02/mandelbrot_mpi_dual.log
echo "" > /home/lmanrique/mac5742/EP02/mandelbrot_mpi_omp_dual.log

# Rodando 15 vezes para as versoes pthreads e openMP
for i in {1..16}; do 
	/home/lmanrique/perf/linux/tools/perf/perf stat -r 15 mpirun -machinefile ~/hosts -n $((4*i)) --display-map -npernode $((2*i)) /home/lmanrique/mac5742/EP02/mandelbrot_mpi >> /home/lmanrique/mac5742/EP02/mandelbrot_mpi_dual.log 2>&1
		for j in {0..6}; do
			/home/lmanrique/perf/linux/tools/perf/perf stat -r 15 mpirun -machinefile ~/hosts -n $((4*i)) --display-map -npernode $((2*i)) /home/lmanrique/mac5742/EP02/mandelbrot_mpi_omp $((2**j)) >> /home/lmanrique/mac5742/EP02/mandelbrot_mpi_omp_dual.log 2>&1
		done
done
