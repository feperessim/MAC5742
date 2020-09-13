OUTPUT=mandelbrot

IMAGE=.ppm

CC=gcc
CC_OPT=-std=c11 

CC_OMP=-fopenmp 
CC_PTH=-pthread 

.PHONY: all
all: $(OUTPUT)_omp_com $(OUTPUT)_pth_com $(OUTPUT)_seq_com $(OUTPUT)_omp_sem $(OUTPUT)_pth_sem $(OUTPUT)_seq_sem

$(OUTPUT)_omp_com: $(OUTPUT)_omp_com.c
	$(CC) -o $(OUTPUT)_omp_com $(CC_OPT) $(CC_OMP) $(OUTPUT)_omp_com.c -lm

$(OUTPUT)_pth_com: $(OUTPUT)_pth_com.c
	$(CC) -o $(OUTPUT)_pth_com $(CC_OPT) $(CC_PTH) $(OUTPUT)_pth_com.c -lm

$(OUTPUT)_seq_com: $(OUTPUT)_seq_com.c
	$(CC) -o $(OUTPUT)_seq_com $(CC_OPT) $(OUTPUT)_seq_com.c

$(OUTPUT)_omp_sem: $(OUTPUT)_omp_sem.c
	$(CC) -o $(OUTPUT)_omp_sem $(CC_OPT) $(CC_OMP) $(OUTPUT)_omp_sem.c -lm

$(OUTPUT)_pth_sem: $(OUTPUT)_pth_sem.c
	$(CC) -o $(OUTPUT)_pth_sem $(CC_OPT) $(CC_PTH) $(OUTPUT)_pth_sem.c -lm

$(OUTPUT)_seq_sem: $(OUTPUT)_seq_sem.c
	$(CC) -o $(OUTPUT)_seq_sem $(CC_OPT) $(OUTPUT)_seq_sem.c

.PHONY: clean
clean:
	rm $(OUTPUT)_omp_com $(OUTPUT)_pth_com $(OUTPUT)_seq_com $(OUTPUT)_omp_sem $(OUTPUT)_pth_sem $(OUTPUT)_seq_sem *$(IMAGE)
