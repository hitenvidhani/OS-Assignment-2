#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
 
long long int dim1;// = 20;
long long int dim2;// = 50;
long long int dim3;// = 20;
long long int *start_idxsA, *start_idxsB;
long long int *matA, *matB;
int num_threadsA = 10, num_threadsB = 10;
char* inputfile1,*inputfile2,*outputfile;
// B ka part
key_t keyB;
int shmidB;

int shmidA;
key_t keyA;

void *readMatrixRowB(void * args){
	long long int *x = (long long int *) args;
	//printf("%lld",*x);
	long long int num; char ch;
	FILE *fp;
	fp = fopen(inputfile2, "r");
	fseek(fp, start_idxsB[*x], SEEK_CUR);
	long long int j = 0;
	while (fscanf(fp, "%lld%c", &num, &ch) == 2) {
            //do something
			matB[dim3 * (*x) + j] = num;
			//printf("%lld ",num);
            if(ch == '\n')
                break;
			j++;
    }
	// printf("line %lld\n",j);
}
 
 
void get_start_idxsB() {
    start_idxsB = (long long int *) malloc(dim2 * sizeof(long long int));
    start_idxsB[0] = 0;
    FILE *fp;
    fp = fopen(inputfile2, "r");
    long long int i = 1;
    char c = getc(fp);
    while(c != EOF) {
    	//printf("%c",getc(fp));
    	while(c != '\n') {
   		//fseek(fp,1,SEEK_CUR);
            //printf("lolololo");
    		// printf("%c",c);
    		c = getc(fp);
    	}
    	//if(c==EOF)break;
    	start_idxsB[i] = ftell(fp);
    	i++;
    	// printf("\n");
    	c = getc(fp);
   }
//    printf("\n");
}

void readSecond() {

	keyB = ftok(".", 'D');
    // printf("%d\n",keyB);
    shmidB = shmget(keyB, dim2 * dim3 * sizeof(long long int), 0666 | IPC_CREAT | IPC_EXCL);
	shmidB = shmget(keyB, dim2 * dim3 * sizeof(long long int), 0);
    // printf("%dabc\n",shmidB);
    matB = (long long int *) shmat(shmidB, (void*) 0, 0);
	memset(matB, -1, dim2 * dim3 * sizeof(long long int));
    
	// printf("hello %lld\n", matB[0]);
	get_start_idxsB();
	// printf("\n");
	pthread_t t[num_threadsB];
	long long int rpt = dim2 / num_threadsB;
	clock_t time;
    time = clock();
	for(long long int i = 0; i < rpt; i++) {
		for(long long int j = 0; j < num_threadsB; j++) {
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threadsB * i + j;
			pthread_create(&t[j], NULL, readMatrixRowB, arg);
		}
		for(long long int j = 0; j < num_threadsB; j++) {
			pthread_join(t[j], NULL);
		}
	}
	long long int left_over_rows = dim2 % num_threadsB;
	
	for(long long int j = 0; j < left_over_rows; j++) {
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threadsB * rpt + j;
			pthread_create(&t[j],  NULL, readMatrixRowB, arg);
	}
	for(long long int j = 0; j < left_over_rows; j++) {
			pthread_join(t[j], NULL);
	}
	// time = clock() - time;
	// double time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds
 
    // printf("took %f seconds to execute \n", time_taken);
	// printf("\n");
	FILE *fp;
	fp = fopen("CopyOfMatB.txt", "w");
	for(long long int i = 0; i < dim2; i++) {
		for(long long int j = 0; j < dim3; j++) {
			fprintf(fp, "%lld ", matB[i * dim3 + j]);
		}
		fputc('\n',fp);
	}

    
    shmdt((void *) matB);

}

 
void *readMatrixRowA(void * args) {
	long long int *x = (long long int *) args;
	long long int num; char ch;
	FILE *fp;
	fp = fopen(inputfile1, "r");
	fseek(fp, start_idxsA[*x], SEEK_CUR);
	long long int j = 0;
	while (fscanf(fp, "%lld%c", &num, &ch) == 2) {
            //do something
			matA[dim2 * (*x) + j] = num;
			//printf("%lld ",num);
            if(ch == '\n')
                break;
			j++;
    }
}
 
 
void get_start_idxsA() {
    start_idxsA = (long long int *)malloc(dim1 * sizeof(long long int));
    start_idxsA[0] = 0;
    FILE *fp;
    fp = fopen(inputfile1, "r");
    long long int i = 1;
    char c = getc(fp);
    while(c != EOF) {
    	while(c != '\n') {
   		
    		// printf("%c",c);
    		c = getc(fp);
    	}
    	start_idxsA[i] = ftell(fp);
    	i++;
    	// printf("\n");
    	c = getc(fp);
   }
//    printf("\n");
}

void readFirst() {

	keyA = ftok(".", 'C');
    // printf("%d\n",keyA);
    shmidA = shmget(keyA, dim1 * dim2 * sizeof(long long int), 0666 | IPC_CREAT | IPC_EXCL);
	shmidA = shmget(keyA, dim1 * dim2 * sizeof(long long int), 0);
    // printf("%dabc\n",shmidA);
    matA = (long long int *)shmat(shmidA, (void*)0, 0);
	memset(matA, -1, dim2 * dim1 * sizeof(long long int));
	
	get_start_idxsA();
	// printf("\n");
	pthread_t t[num_threadsA];
	long long int rpt = dim1 / num_threadsA;
	// clock_t time;
    // time = clock();
	for(long long int i = 0; i < rpt; i++){
		for(long long int j = 0;j < num_threadsA; j++) {
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threadsA * i + j;
			pthread_create(&t[j], NULL, readMatrixRowA, arg);
		}
		for(long long int j = 0; j < num_threadsA; j++) {
			pthread_join(t[j], NULL);
		}
	}
	long long int left_over_rows = dim1 % num_threadsA;
	
	for(long long int j = 0; j < left_over_rows; j++) {
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threadsA * rpt + j;
			pthread_create(&t[j], NULL, readMatrixRowA, arg);
	}
	for(long long int j = 0; j < left_over_rows; j++) {
			pthread_join(t[j], NULL);
	}
	// time = clock() - time;
	// double time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds
 
    // printf("took %f seconds to execute \n", time_taken);
	// printf("\n");
	FILE *fp;
	fp = fopen("CopyOfMatA.txt", "w");
	for(long long int i = 0; i < dim1; i++) {
		for(long long int j = 0; j < dim2; j++) {
			fprintf(fp, "%lld ", matA[i * dim2 + j]);
		}
		fputc('\n', fp);
	}

    
    shmdt((void *) matA);

}
 
 
int main(int argc,char *argv[])
{
    dim1 = atoll(argv[1]);
    dim2 = atoll(argv[2]);
    dim3 = atoll(argv[3]);
	inputfile1 = argv[4];
	inputfile2 = argv[5];
	outputfile = argv[6]; 


	
	if(num_threadsA > dim1){
		num_threadsA = dim1;
	}

	if(num_threadsB > dim2){
		num_threadsB = dim2;
	}

    keyB = ftok(".", 'D');
    // printf("%d\n",keyB);
    shmidB = shmget(keyB, dim2 * dim3 * sizeof(long long int), 0666 | IPC_CREAT | IPC_EXCL);
	shmidB = shmget(keyB, dim2 * dim3 * sizeof(long long int), 0);
    // printf("%dabc\n",shmidB);
    matB = (long long int *)shmat(shmidB, (void*)0, 0);
	memset(matB, -1, dim2 * dim3 * sizeof(long long int));

    keyA = ftok(".", 'C');
    // printf("%d\n",keyA);
    shmidA = shmget(keyA, dim1 * dim2 * sizeof(long long int), 0666 | IPC_CREAT | IPC_EXCL);
	shmidA = shmget(keyA, dim1 * dim2 * sizeof(long long int), 0);
    // printf("%dabc\n",shmidA);
    matA = (long long int *)shmat(shmidA, (void*)0, 0);
	memset(matA, -1, dim2*dim1*sizeof(long long int));

    readSecond();
    readFirst();

    
    // shmdt((void *) matA);
	printf("P1 ends here\n");
	kill(getppid(), SIGUSR1);
	return 0;
}