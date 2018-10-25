#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

void* threadFunction(void *var) {
	int thread_id = *(int*)var;

	printf("Hello world from thread %i\n", thread_id);
}

void * secondFunction(void * var) {

	int thread_id = *(int *)var;
	printf("This is the second function from thread number: %i\n", thread_id);
}



int main(int argc, char **argv) {
	int P = sysconf(_SC_NPROCESSORS_CONF);
	int i;
	// int P = 2;

	pthread_t tid[P];
	int thread_id[P];
	
	for(i = 0;i < P; i++)
		thread_id[i] = i;

	for (i = 0;i < P; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}
	// pthread_create(&(tid[1]), NULL, secondFunction, &(thread_id[1]));


	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	printf("Number of cores: %d\n",sysconf(_SC_NPROCESSORS_CONF));

	return 0;
}

