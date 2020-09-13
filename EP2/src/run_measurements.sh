#! /bin/bash

set -o xtrace

MEASUREMENTS=10
ITERATIONS=10
INITIAL_SIZE=16

SIZE=$INITIAL_SIZE

NAMES=('mandelbrot_seq_com' 'mandelbrot_seq_sem' 'mandelbrot_pth_com' 'mandelbrot_pth_sem' 'mandelbrot_omp_com' 'mandelbrot_omp_sem')

make
mkdir results

for NAME in ${NAMES[@]}; do
    mkdir results/$NAME

    for ((i=1; i<=$ITERATIONS; i++)); do
    		if [ "$NAME" = 'mandelbrot_seq_sem' ] || [ "$NAME" = 'mandelbrot_seq_com' ]; then
	    	    perf stat -r $MEASUREMENTS ./$NAME -2.5 1.5 -2.0 2.0 $SIZE 1 >> full.log 2>&1
	            perf stat -r $MEASUREMENTS ./$NAME -0.8 -0.7 0.05 0.15 $SIZE 1 >> seahorse.log 2>&1
	            perf stat -r $MEASUREMENTS ./$NAME 0.175 0.375 -0.1 0.1 $SIZE 1 >> elephant.log 2>&1
	            perf stat -r $MEASUREMENTS ./$NAME -0.188 -0.012 0.554 0.754 $SIZE 1 >> triple_spiral.log 2>&1
    		else
    			for ((j=1; j<=32; j = j * 2)); do
    				if [ "$NAME" = 'mandelbrot_omp_sem' ] || [ "$NAME" = 'mandelbrot_omp_com' ]; then
    					export OMP_NUM_THREADS=$j
						perf stat -r $MEASUREMENTS ./$NAME -2.5 1.5 -2.0 2.0 $SIZE $j >> full.log 2>&1
						perf stat -r $MEASUREMENTS ./$NAME -0.8 -0.7 0.05 0.15 $SIZE $j >> seahorse.log 2>&1
						perf stat -r $MEASUREMENTS ./$NAME 0.175 0.375 -0.1 0.1 $SIZE $j >> elephant.log 2>&1
						perf stat -r $MEASUREMENTS ./$NAME -0.188 -0.012 0.554 0.754 $SIZE $j >> triple_spiral.log 2>&1
    				else
						perf stat -r $MEASUREMENTS ./$NAME -2.5 1.5 -2.0 2.0 $SIZE $j >> full.log 2>&1
						perf stat -r $MEASUREMENTS ./$NAME -0.8 -0.7 0.05 0.15 $SIZE $j >> seahorse.log 2>&1
						perf stat -r $MEASUREMENTS ./$NAME 0.175 0.375 -0.1 0.1 $SIZE $j >> elephant.log 2>&1
						perf stat -r $MEASUREMENTS ./$NAME -0.188 -0.012 0.554 0.754 $SIZE $j >> triple_spiral.log 2>&1
				fi
    			done
    		fi
  

            SIZE=$(($SIZE * 2))
    done

    SIZE=$INITIAL_SIZE

    mv *.log results/$NAME
    rm output.ppm
done
