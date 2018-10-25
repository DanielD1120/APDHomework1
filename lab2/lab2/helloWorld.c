#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

int a = 0;
//pthread_mutex_t mutex;
pthread_barrier_t barrier;

void* threadFunction(void *var) {
	//int * a = (int*)var;
	/*pthread_mutex_lock(&mutex);
	a += 2;
	pthread_mutex_unlock(&mutex);*/
	printf("1\n");
	pthread_barrier_wait(&barrier);
}

void * secondFunction(void * var) {
	//int * a = (int *)var;
	/*pthread_mutex_lock(&mutex);
	a += 3;
	pthread_mutex_unlock(&mutex);*/
	pthread_barrier_wait(&barrier);
	printf("2\n");
}



int main(int argc, char **argv) {
	//int P = sysconf(_SC_NPROCESSORS_CONF);
	int i;
	int P = 2;
	//int a;

	//pthread_mutex_init(&mutex,NULL);
	pthread_barrier_init(&barrier,NULL,2);
	
	pthread_t tid[P];
	
	for(i = 0;i < P; i++)
		tid[i] = i;

	pthread_create(&(tid[0]), NULL, threadFunction, NULL);
	pthread_create(&(tid[1]), NULL, secondFunction, NULL);


	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	//printf("a: %d\n",a);
	//pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);

	return 0;
}

