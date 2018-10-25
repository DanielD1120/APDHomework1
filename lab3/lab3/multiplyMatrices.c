#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

int NReps;
int printLevel;
int N;
int** a;
int** b;
int** c;
pthread_mutex_t mutex;

typedef struct {
	int start;
	int end;
} Indexes;


void* multiplyElementsLine(void *var)
{
	Indexes ind = *(Indexes *) var;

	int i1 = ind.start;
	int i2 = ind.end;

	for(int i = i1; i <= i2; i++) {
		for(int j = 0; j < N; j++) {
			for(int k = 0; k < N; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

}

void* multiplyElementsColumn(void *var)
{
	Indexes ind = *(Indexes *) var;

	int i1 = ind.start;
	int i2 = ind.end;

	for(int i = 0; i < N; i++) {
		for(int j = i1; j <=i2 ; j++) {
			for(int k = 0; k < N; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

void* multiplyElementsInner(void *var)
{
	Indexes ind = *(Indexes *) var;

	int i1 = ind.start;
	int i2 = ind.end;
	int tmp;

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N ; j++) {
			for(int k = i1; k <= i2; k++) {
				tmp += a[i][k] * b[k][j];
			}
			pthread_mutex_lock(&mutex);
			c[i][j] += tmp;
			pthread_mutex_unlock(&mutex);
			tmp = 0;
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
	a = malloc(sizeof(int *) * N);
	b = malloc(sizeof(int *) * N);
	c = malloc(sizeof(int *) * N);
	
	if(a == NULL || b == NULL || c == NULL) {
		printf("malloc failed!");
		exit(1);
	}

	int i, j;
	for(i = 0; i < N; i++) {
		a[i] = malloc(sizeof(int) * N);
		b[i] = malloc(sizeof(int) * N);
		c[i] = malloc(sizeof(int) * N);
		if(a[i] == NULL || b[i] == NULL || c[i] == NULL) {
			printf("malloc failed!");
			exit(1);
		}
	}

	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			c[i][j] = 0;
			if(i <= j) {
				a[i][j] = 1;
				b[i][j] = 1;
			}
			else {
				a[i][j] = 0;
				b[i][j] = 0;
			}
		}
	}
}

void printAll()
{
	int i, j;
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			printf("%i\t",c[i][j]);
		}
		printf("\n");
	}
}

void printPartial()
{
	printAll();
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
	int i, j, k;
	getArgs(argc, argv);
	init();

	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			for(k = 0; k < N; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	printAll();
	printf("\n\nL");

	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			for(k = 0; k < N; k++) {
				c[i][j] = 0;
			}
		}
	}

	int P = (int) sysconf(_SC_NPROCESSORS_ONLN);
	printf("Number of threads: %d\nLines:\n",P );
    pthread_t tid[P];
   
    Indexes *elements;
    elements = (Indexes*) malloc(P * sizeof(Indexes));
   
    int elements_number = N / (P - 1);
 
    for(i = 0; i < P - 1; i++) {
        tid[i] = i;
        elements[i].start = i * elements_number;
        elements[i].end = (i + 1) * elements_number - 1;
        pthread_create(&(tid[i]), NULL, multiplyElementsLine, &(elements[i]));
    }
 
    tid[P - 1] = P - 1;
    elements[P - 1].start = (P - 1) * elements_number;
    elements[P - 1].end = N - 1;
    pthread_create(&(tid[P - 1]), NULL, multiplyElementsLine, &(elements[P - 1]));
 
    for(i = 0; i < P; i++) {
        pthread_join(tid[i], NULL);
    }
 
    printAll();
    printf("\n\nColumns:\n");

	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			for(k = 0; k < N; k++) {
				c[i][j] = 0;
			}
		}
	}

 
    for(i = 0; i < P - 1; i++) {
        tid[i] = i;
        elements[i].start = i * elements_number;
        elements[i].end = (i + 1) * elements_number - 1;
        pthread_create(&(tid[i]), NULL, multiplyElementsColumn, &(elements[i]));
    }
 
    tid[P - 1] = P - 1;
    elements[P - 1].start = (P - 1) * elements_number;
    elements[P - 1].end = N - 1;
    pthread_create(&(tid[P - 1]), NULL, multiplyElementsColumn, &(elements[P - 1]));
 
    for(i = 0; i < P; i++) {
        pthread_join(tid[i], NULL);
    }
 
    printAll();
    printf("\n\nInner\n");

    for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			for(k = 0; k < N; k++) {
				c[i][j] = 0;
			}
		}
	}

 
    for(i = 0; i < P - 1; i++) {
        tid[i] = i;
        elements[i].start = i * elements_number;
        elements[i].end = (i + 1) * elements_number - 1;
        pthread_create(&(tid[i]), NULL, multiplyElementsInner, &(elements[i]));
    }
 
    tid[P - 1] = P - 1;
    elements[P - 1].start = (P - 1) * elements_number;
    elements[P - 1].end = N - 1;
    pthread_create(&(tid[P - 1]), NULL, multiplyElementsInner, &(elements[P - 1]));
 
    for(i = 0; i < P; i++) {
        pthread_join(tid[i], NULL);
    }

    printAll();
    printf("\n\n");



	return 0;
}