#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <pthread.h>

int NReps;
int printLevel;
int N;
int* v;
int *vQSort;
int *vNew;
int P;
int** a;

pthread_barrier_t barrier;


int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}


int cmp2(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return B-A;
}

void* threadFunction(void* var) {
	int k = *(int*)var;
	int i, j, i1, j1;
	int* aux;
	aux = (int*)malloc((sqrt(N)) * sizeof(int));
	for(i=0; i<log(N)+2; i++) {
		if(i%2 == 0) {
			if(k % 2 == 0) {
				qsort(a[k], sqrt(N), sizeof(int), cmp);
			} else {
				qsort(a[k], sqrt(N), sizeof(int), cmp2);
			}
			pthread_barrier_wait(&barrier);
		} else {
			for(j=0; j<sqrt(N); j++) {
				aux[j] = a[j][k];
			}
			qsort(aux, sqrt(N), sizeof(int), cmp);
			for(j=0; j<sqrt(N); j++) {
				a[j][k] = aux[j];
			}
			pthread_barrier_wait(&barrier);
		}
	}
}

void compareVectors(int * a, int * b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		if(a[i]!=b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++)
		printf("%d ", v[i]);
	printf("\n");
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
	int i;
	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);
	vNew = malloc(sizeof(int) * N);
	a = (int**)malloc(sqrt(N)*sizeof(int*));
	for(i = 0; i < N; i++) {
		a[i] = (int*)malloc(sqrt(N) * sizeof(int));
	}
	if(v == NULL) {
		printf("malloc failed!");
		exit(1);
	}

	// generate the vector v with random values
	// DO NOT MODIFY
	srand(42);
	for(i = 0; i < N; i++)
		v[i] = rand()%N;
}

void printPartial()
{
	int i;
	compareVectors(v, vQSort);
}

void printAll()
{
	displayVector(v);
	displayVector(vQSort);
	compareVectors(v, vQSort);
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
	int i, j, aux;
	getArgs(argc, argv);
	init();

	// make copy to check it against qsort
	// DO NOT MODIFY
	for(i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	int k = 0;
	for(i = 0; i < sqrt(N); i++) {
		for(j = 0; j < sqrt(N); j++) {
			a[i][j] = v[k++];
		}
	}
	
	// for(i = 0; i < sqrt(N); i++) {
	// 	for(j = 0; j < sqrt(N); j++) {
	// 		printf("%d ", a[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n");

	P = sqrt(N);
	pthread_t tid[P];
	int thread_id[P];
	for(i = 0; i < P; i++)
		thread_id[i] = i;

	pthread_barrier_init(&barrier, NULL, P);

	for(i = 0; i < P; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	pthread_barrier_destroy(&barrier);

	// printf("\n");
	// for(i = 0; i < sqrt(N); i++) {
	// 	for(j = 0; j < sqrt(N); j++) {
	// 		printf("%d ", a[i][j]);
	// 	}
	// 	printf("\n");
	// }

	k=0;

	for (int i = 0; i < sqrt(N) ;i++) {
		if (i % 2 == 0) {
			for (int j = 0; j < sqrt(N); j++) {
				v[k++] = a[i][j];
			}
		} else {
			for (int j = sqrt(N)-1; j >= 0; j--) {
				v[k++] = a[i][j];
			}
		}
	}
	//printf("\n");
	print();

	return 0;
}