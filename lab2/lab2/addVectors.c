#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int NReps;
int printLevel;
int N;
int* a;
int* b;
int* c;

typedef struct vects {
	int * a;
	int * b;
	int * c;
	int nr_reps;
	int i1;
	int i2;
} Vectori;

void* threadFunction(void *var)
{
	Vectori vects = *(Vectori *) var;

	int * v1 = vects.a;
	int * v2 = vects.b;
	int * v3 = vects.c;
	int i1 = vects.i1;
	int i2 = vects.i2;
	int nr_reps = vects.nr_reps;
	
	for (int j = 0; j < nr_reps;j++){
		for (int i = i1; i <= i2; i++) {
			v3[i]+=v1[i]+v2[i];
		}
	}

}

void getArgs(int argc, char **argv)
{
	if(argc < 4) {
		printf("Not enough paramters: ./program N NReps printLevel\nprintLevel: 0=no, 1=some, 2=verbouse\n");
		exit(1);
	}
	N = atoi(argv[1]);
	NReps = atoi(argv[2]);
	printLevel = atoi(argv[3]);
}

void init()
{
	a = malloc(sizeof(int) * N);
	b = malloc(sizeof(int) * N);
	c = malloc(sizeof(int) * N);
	if(a == NULL || b == NULL || c == NULL) {
		printf("malloc failed!");
		exit(1);
	}

	int i;
	for(i = 0; i < N; i++) {
		a[i] = i%500;
		b[i] = i%500;
	}
}

void printPartial()
{
	int i;
	for(i = 0; i < 10; i++)
		printf("%i ", c[i]);
	printf("\n");
	for(i = 20; i < N; i+=N/10)
		printf("%i ", c[i]);
	printf("\n");
	for(i = N - 10; i < N; i++)
		printf("%i ", c[i]);
	printf("\n");
}

void printAll()
{
	int i;
	for(i = 0; i < N; i++)
		printf("%i ", c[i]);
	printf("\n");
}

void print()
{
	if(printLevel == 0)
		return;
	else if(printLevel == 1)
		printPartial();
	else
		printAll();
}

int main(int argc, char *argv[])
{
	int i, j;
	getArgs(argc, argv);
	init();

	int P = 4;
	int exact = 0;
	int k = 0;
	pthread_t tid[P];
	Vectori thread_id[P];

	if ((N%P)==0) {
		exact = 1;
		k = N/P;
	} else {
		exact = 0;
		k = N/P;
	}

	for (i = 0;i < P; i++) {
		if (exact) {
			thread_id[i].a = a;
			thread_id[i].b = b;
			thread_id[i].c = c;
			thread_id[i].nr_reps = NReps;
			thread_id[i].i1 = k*i;
			thread_id[i].i2 = k*(i+1)-1;
			printf("%d %d\n",thread_id[i].i1,thread_id[i].i2 );
		} else {
			if (i!=P-1) {
				thread_id[i].a = a;
				thread_id[i].b = b;
				thread_id[i].c = c;
				thread_id[i].nr_reps = NReps;
				thread_id[i].i1 = k*i;
				thread_id[i].i2 = k*(i+1)-1;
				printf("%d %d\n",thread_id[i].i1,thread_id[i].i2 );
			} else {
				thread_id[i].a = a;
				thread_id[i].b = b;
				thread_id[i].c = c;
				thread_id[i].nr_reps = NReps;
				thread_id[i].i1 = k*i;
				thread_id[i].i2 = N;
				printf("%d %d\n",thread_id[i].i1,thread_id[i].i2 );
			}
		}
	}

	for (i = 0; i < P; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	print();

	return 0;
}
