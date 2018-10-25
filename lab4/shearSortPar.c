#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

int NReps;
int printLevel;
int N;
int sqrtN;
int* v;
int *vQSort;
int *vNew;
int ** vMatrix;
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

void * threadFunction(void * var) {
	int start = *(int *)var;
	int aux[sqrtN];
	for (int i = 0;i < sqrtN;i++) {
		if (i%2 == 0) {

			if (start % 2 == 0) {
				qsort(vMatrix[start], sqrtN, sizeof(int), cmp);
			} else {
				qsort(vMatrix[start], sqrtN, sizeof(int), cmp2);
			}

			pthread_barrier_wait(&barrier);
		
		} else {
			
			for (int k = 0;k < sqrtN;k++) {
				aux[k] = vMatrix[k][start];
			}
			
			qsort(aux, sqrtN, sizeof(int), cmp);
			
			for (int k = 0;k < sqrtN;k++) {
				vMatrix[start][k] = aux[k];
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
	int displayWidth = 2 + log10(v[N-1]);
	for(i = 0; i < N; i++) {
		printf("%*i", displayWidth, v[i]);
		if(!((i+1) % 20))
			printf("\n");
	}
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
	sqrtN = sqrt(N);
	printf("%d\n",sqrtN );
	vMatrix = (int **)malloc(sizeof(int *) * sqrtN);
	if(v == NULL) {
		printf("malloc failed!");
		exit(1);
	}

	// generate the vector v with random values
	// DO NOT MODIFY
	srand(42);
	for(i = 0; i < N; i++) {
		v[i] = rand()%N;
		vMatrix[i] = malloc (sizeof(int) * sqrtN);
	}
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

	int k = 0;
	for (int i = 0;i < sqrtN;i++) {
		for (int j =0;j < sqrtN;j++)
			vMatrix[i][j] = v[k++];
	}

	for (int i = 0;i < sqrtN;i++) {
		for (int j =0;j < sqrtN;j++)
			printf("%d  ",vMatrix[i][j]);
		printf("\n");
	}


	// make copy to check it against qsort
	// DO NOT MODIFY
	for(i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);


	int P = sqrtN;

	pthread_barrier_init(&barrier,NULL,P);
	pthread_t tid[P];

	int auxp[N];

	for(i = 0;i < N; i++)
		auxp[i] = i;
	
	for(i = 0;i < P; i++)
		tid[i] = i;

	for (i = 0;i < P; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(auxp[i]));
	}

	for (i = 0;i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	pthread_barrier_destroy(&barrier);

	printf("\n");
	for (int i = 0;i < sqrtN;i++) {
		for (int j =0;j < sqrtN;j++)
			printf("%d  ",vMatrix[i][j]);
		printf("\n");
	}

	printAll();


	return 0;
}
