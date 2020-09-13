#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <float.h>
#include <sys/time.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef VERBOSE
#define VERBOSE 0
#endif

#define FUNCTIONS 1

struct timer_info {
    clock_t c_start;
    clock_t c_end;
    struct timespec t_start;
    struct timespec t_end;
    struct timeval v_start;
    struct timeval v_end;
};

struct timer_info timer;

char *usage_message = "usage: ./monte_carlo SAMPLES FUNCTION_ID N_THREADS\n";

struct function {
    long double (*f)(long double);
    long double interval[2];
};

long double rand_interval[] = {0.0, (long double) RAND_MAX};

long double f1(long double x){
  return 2 / (sqrt(1 - (x * x))); 
}

struct function functions[] = {
                               {&f1, {0.0, 1.0}}
};

// Your thread data structures go here

struct thread_data {
    long double (*f)(long double);
    long double *samples;
    int size;
    long thread_id;
    long double partial_sum;
};

struct thread_data *thread_data_array;

// End of data structures

long double *samples;
long double *results;

long double map_intervals(long double x, long double *interval_from, long double *interval_to){
    x -= interval_from[0];
    x /= (interval_from[1] - interval_from[0]);
    x *= (interval_to[1] - interval_to[0]);
    x += interval_to[0];
    return x;
}

long double *uniform_sample(long double *interval, long double *samples, int size){
    for(int i = 0; i < size; i++){
        samples[i] = map_intervals((long double) rand(),
                                   rand_interval,
                                   interval);
	      if (samples[i] > (1 - LDBL_EPSILON))
		i--;
		
    }
    return samples;
}

void print_array(long double *sample, int size){
    printf("array of size [%d]: [", size);

    for(int i = 0; i < size; i++){
        printf("%Lf", sample[i]);

        if(i != size - 1){
            printf(", ");
        }
    }

    printf("]\n");
}

long double monte_carlo_integrate(long double (*f)(long double), long double *samples, int size){
    // Your sequential code goes here

    int i = 0;
    long double sum = 0.0;
 
    for (i = 0; i < size; i++) {
      sum += f(samples[i]);
    }
   
    return sum/size;
}

void *monte_carlo_integrate_thread(void *args){
    // Your pthreads code goes here
    struct thread_data *my_data;
    int i;
    //long thread_id;
    my_data = (struct thread_data *) args;
    
    for (i = 0; i < my_data->size; i++) {
      my_data->partial_sum += my_data->f(my_data->samples[i]);
    }
  
    pthread_exit(NULL);
}

int main(int argc, char **argv){
    if(argc != 4){
        printf("%s", usage_message);
        exit(-1);
    } else if(atoi(argv[2]) >= FUNCTIONS || atoi(argv[2]) < 0){
        printf("Error: FUNCTION_ID must in [0,%d]\n", FUNCTIONS - 1);
        printf("%s", usage_message);
        exit(-1);
    } else if(atoi(argv[3]) < 0){
        printf("Error: I need at least 1 thread\n");
        printf("%s", usage_message);
        exit(-1);
    }

    if(DEBUG){
        printf("Running on: [debug mode]\n");
        printf("Samples: [%s]\n", argv[1]);
        printf("Function id: [%s]\n", argv[2]);
        printf("Threads: [%s]\n", argv[3]);
        printf("Array size on memory: [%.2LFGB]\n", ((long double) atoi(argv[1]) * sizeof(long double)) / 1000000000.0);
    }

    srand(time(NULL));
    int size = atoi(argv[1]);
    struct function target_function = functions[atoi(argv[2])];
    int n_threads = atoi(argv[3]);

    if (size <= 0) {
	printf("Error: size <= 0");
	exit(-1);
    }

    samples = malloc(size * sizeof(long double));

    if (samples == NULL) {
	printf("Error: Could not allocate space - 'thread_data_array'\n");
	exit(-1);
    }
    
    long double estimate = 0.0;

    if(n_threads == 1){
        if(DEBUG){
            printf("Running sequential version\n");
        }

	timer.c_start = clock();
        clock_gettime(CLOCK_MONOTONIC, &timer.t_start);
        gettimeofday(&timer.v_start, NULL);

        estimate = monte_carlo_integrate(target_function.f,
                                         uniform_sample(target_function.interval,
                                                        samples,
                                                        size),
                                         size);

        timer.c_end = clock();
        clock_gettime(CLOCK_MONOTONIC, &timer.t_end);
        gettimeofday(&timer.v_end, NULL);
    } else {
        if(DEBUG){
            printf("Running parallel version\n");
        }
	pthread_t *threads;
	pthread_attr_t attr; 

	thread_data_array = malloc(n_threads * sizeof(struct thread_data));
	if (thread_data_array == NULL) {
	    printf("Error: Could not allocate space - 'thread_data_array'\n");
	    exit(-1);
	}

	threads = malloc(n_threads * sizeof (pthread_t));
	if (thread_data_array == NULL) {
	    free(thread_data_array);
	    printf("Error: Could not allocate space - '*threads'\n");
	    exit(-1);
	}

	// We set the strategy 1 - n threads with m works
	int m = size / n_threads;
	int remainder = size % n_threads;
	int i;

	
	timer.c_start = clock();
        clock_gettime(CLOCK_MONOTONIC, &timer.t_start);
        gettimeofday(&timer.v_start, NULL);

	samples = uniform_sample(target_function.interval,
				 samples,
				 size);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        // Your pthreads code goes here
	int rc;
	for(i=0; i<n_threads; i++) {
	  thread_data_array[i].f = target_function.f;
	  thread_data_array[i].samples = samples + m*i;
	  thread_data_array[i].size = m;
	  thread_data_array[i].partial_sum = 0.0;
	  if (i == n_threads - 1 && remainder > 0) {
	    thread_data_array[n_threads - 1].size +=  remainder;
	  }
	    //printf("Main: creating thread %d\n", i);

	  if ((rc = pthread_create(&threads[i], &attr, monte_carlo_integrate_thread, (void *)&thread_data_array[i]))) {
	    printf("ERROR; return code from pthread_create() is %d\n", rc);
	  }
	}

	void *status;
	pthread_attr_destroy(&attr);
	for(i=0; i<n_threads; i++) {
	    rc = pthread_join(threads[i], &status);
	    if (rc) {
		printf("ERROR; return code from pthread_join() is %d\n", rc);
		// do something here
	    }
	    else {
		estimate += thread_data_array[i].partial_sum;
	    }
	    //printf("Main: completed join with thread %d having a status of %ld\n",i,(long)status); 
	}
        // Your pthreads code ends here
	estimate = estimate / size;
	timer.c_end = clock();
        clock_gettime(CLOCK_MONOTONIC, &timer.t_end);
        gettimeofday(&timer.v_end, NULL);
	results = samples;
	free(thread_data_array);
	free(threads);

        if(DEBUG && VERBOSE){
            print_array(results, n_threads);
        }
    }

    if(DEBUG){
        if(VERBOSE){
            print_array(samples, size);
            printf("Estimate: [%.33LF]\n", estimate);
        }
        printf("%.16LF, [%f, clock], [%f, clock_gettime], [%f, gettimeofday]\n",
               estimate,
               (double) (timer.c_end - timer.c_start) / (double) CLOCKS_PER_SEC,
               (double) (timer.t_end.tv_sec - timer.t_start.tv_sec) +
               (double) (timer.t_end.tv_nsec - timer.t_start.tv_nsec) / 1000000000.0,
               (double) (timer.v_end.tv_sec - timer.v_start.tv_sec) +
               (double) (timer.v_end.tv_usec - timer.v_start.tv_usec) / 1000000.0);
    } else {
        printf("%.16LF, %f\n",
               estimate,
               (double) (timer.t_end.tv_sec - timer.t_start.tv_sec) +
               (double) (timer.t_end.tv_nsec - timer.t_start.tv_nsec) / 1000000000.0);
    }
    return 0;
}
