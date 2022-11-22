#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<string.h>

// int to long long int
// get all the matrices from shared memory so you can access using indexes
// 
//long long int *A; //[20001];
//long long int *B; //[60001];
//long long int *C; //[30001]={0};
long long int size_r = 20;
long long int size_c = 50;
long long int *matA, *matB, *matC;
long long int dim1 = 20;
long long int dim2 = 50;
long long int dim3 = 20;


int shmidA;
long long int *matA;

int shmidB;
long long int *matB;

key_t keyA;
key_t keyB;
 
//int dim1, dim2, dim2, dim3;
long long int rpt;

//      col*r + c;


//     int r = A / col;
//     int c = A % col;
 
typedef struct{
	long long int st,ed;
} pair;

 
void *matrixMul(void *args) {
    pair *x = (pair *) args;
    printf("thread %lld : st-%lld, ed-%lld\n", (x->st)/rpt, x->st, x->ed);

    keyA = ftok(".", 'A');
    //shmidA = shmget(keyA, dim1*dim2*sizeof(long long int), IPC_EXCL);
    matA = shmat(shmidA, 0, SHM_RDONLY);

    keyB = ftok(".", 'B');
    //shmidB = shmget(keyB, dim3*dim2*sizeof(long long int), IPC_EXCL);
    matB = shmat(shmidB, 0, SHM_RDONLY);
    
    //TO CHECK IF ROWS TO BE READ HAVE BEEN WRITTEN
    //If the last element of a row is -1, then you can't read the row, if it's positive, we can be certain that entire row is read
    // int b=0;
    // while(b==0) {
    //     b=1;
    //     for(int i = x->st; i < x->ed; i++) {
    //         if(A[dim2*i+dim2-1]==-1)
    //             b=0;
    //     }
    //     printf("in while loop\n");
    // }
    //CALCULATIONS
    printf("%lld\ndebug\n", matA[0]);
    for (long long int i = x->st; i < x->ed; i++) {
        for (long long int j = 0; j < dim3; j++) { //dim 3
            for (long long int k = 0; k < dim2; k++) { // dim 2
                matC[dim3*i+j] += matA[dim2*i+k] * matB[dim3*k+j]; //      g += 
                // printf("A in i : %d,  j : %d , k : %d is : %lld\n", i, j, k,  A[dim2*i+k]);
                // printf("B in i : %d,  j : %d , k : %d is : %lld\n", i, j, k,  B[dim2*i+k]);
            }
        }
    }
 
}
 
int main(int argc, char *argv[]) {

    if (argc != 4) {
		printf("Wrong Usage");
		exit(-1);
	}
    dim1 = atoll(argv[1]);
    dim2 = atoll(argv[2]);
    dim3 = atoll(argv[3]);

    matC = (long long int *)malloc(sizeof(long long int)* dim1 * dim3);
    memset(matC, 0, dim1*dim3*sizeof(long long int));

    keyA = ftok(".", 'A');
    shmidA = shmget(keyA, dim1*dim2*sizeof(long long int), IPC_EXCL);
    matA = shmat(shmidA, 0, SHM_RDONLY);
    printf("hello %lld hello2 %lld \n", matA[10], matA[20]);
    
    //shmctl(shmidA, IPC_RMID, NULL);

    key_t keyB = ftok(".", 'B');
    shmidB = shmget(keyB, dim3*dim2*sizeof(long long int), IPC_EXCL);
    matB = shmat(shmidB, 0, SHM_RDONLY);
    printf("hello %lld hello2 %lld \n", matB[10], matB[20]);
    
    //shmctl(shmidB, IPC_RMID, NULL);

 
    struct timeval start, end;
    gettimeofday(&start, NULL);
 
    //dim1 = 20;
    //dim2 = dim2 = 50;
    //dim3 = 20;
 
    // for(int i=0;i<dim2*dim1;i++)
    //     A[i]=i+1;
 
    // for(int i=0;i<dim3*dim2;i++)
    //     B[i]=i+1;
    // long long int num;
    // char ch;
    // long long int j=0;
    // FILE *inpfile1;
    // inpfile1 = fopen("in2.txt", "r");
    // j=0;
    // for(int x=0;x<dim2;x++) {
    //     j=0;
    //     while (fscanf(inpfile1, "%lld%c", &num, &ch)==2){
    //         //do something
	// 		B[dim3*x+j] = num;
    //         if(ch == '\n' | ch==EOF)
    //             break;
	//     	j++;
    //     }
    // }
    // fclose(inpfile1);

    // FILE *inpfile;
    // inpfile = fopen("in1.txt", "r");
    
    // for(int x=0;x<dim1;x++) {
    //     j=0;
    //     while (fscanf(inpfile, "%lld%c", &num, &ch)==2){
    //         //do something
	// 		A[dim2*(x)+j] = num;
    //         if(ch == '\n')
    //             break;
	//     	j++;
    //     }
    // }
    // fclose(inpfile);
    
    printf("AAAAAAAAAAAAAAAAAA\n");
    for (long long int i = 0; i < dim1; i++) {
        for (long long int j = 0; j < dim2; j++) {
            //long long int k = matA[i*dim2+j];
            printf("%lld ", matA[i*dim2+j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("BBBBBBBBBBBBBBBBBB\n");
    for (long long int i = 0; i < dim2; i++) {
        for (long long int j = 0; j < dim3; j++) {
            printf("%lld ", matB[i*dim3+j]);
        }
        printf("\n");
    }
    printf("\n");
 
    //max threads : one thread for each row of C matrix.
    //NOTE: creating one thread for each element will be inefficient 
    pthread_t threads[100];
 
    //one thread for each column
 
    int num_threads = 3;

    if(num_threads>dim1){
        num_threads=dim1;
    }
 
    //num_threads = num_threads > dim1 ? dim1 : num_threads;
 
    rpt = (dim1 / num_threads) + 1; // rows per thread

    long long int ceil_rpt = dim1 / num_threads + 1; //4
    long long int flr_rpt = dim1 / num_threads; //3

    long long int rem = dim1 % num_threads; // 1

    //num threads = 2, dim1s = 5
    // 10 threads x 4 rows and 20 threads x 3 rows
 
    for(long long int l = 0 ; l<rem ; l++) { //
        //INITIALIZE ARRAY OF ARG BEFORE HAND?
        pair * arg = (pair *) malloc(sizeof(pair));
        arg->st = l * ceil_rpt;
        arg->ed = (l+1) * ceil_rpt;
        printf("in main 1 : thread %lld : st-%lld, ed-%lld\n", l, arg->st, arg->ed);
        pthread_create(&threads[l], NULL, matrixMul, (void *) arg);
    }

    printf("ceil %lld\n", rem*ceil_rpt);
    printf("l lim %lld\n", num_threads-rem);
    for(long long int l = rem; l<num_threads; l++) {
        pair * arg = (pair *) malloc(sizeof(pair));
        arg->st = l * flr_rpt + rem * (ceil_rpt-flr_rpt);
        arg->ed = (l+1) * flr_rpt + rem * (ceil_rpt-flr_rpt);
        printf("in main 2 : thread %lld : st-%lld, ed-%lld\n", l, arg->st, arg->ed);
        pthread_create(&threads[l], NULL, matrixMul, (void *) arg);
    }

 
    for (long long int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    for (long long int i = 0; i < dim1; i++) {
        for (long long int j = 0; j < dim3; j++)
            printf("%lld ", matC[i*dim3+j]);
        printf("\n");
    }
 
    FILE *opfile;
    opfile = fopen("output.txt", "w");
    for (long long int i = 0; i < dim1; i++) {
        for (long long int j = 0; j < dim3; j++) {
            fprintf(opfile, "%lld ", matC[i*dim3+j]);
        }
        fputc('\n', opfile);
    }
 
    gettimeofday(&end, NULL);
 
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
 
    printf("The elapsed time is %ld seconds and %ld micros\n", seconds, micros);
    
    shmdt((void *) matA);
    shmdt((void *) matB);
	return 0;
}