#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<string.h>
#include <unistd.h>
#include <signal.h>

// int to long long int
// get all the matrices from shared memory so you can access using indexes
// 
//long long int *A; //[20001];
//long long int *B; //[60001];
//long long int *C; //[30001]={0};

#define BIL 1000000000LL
#define MIL 1000000LL

struct timespec calcTimeDiff(struct timespec end, struct timespec start) {
    struct timespec diff;

    if(end.tv_nsec >= start.tv_nsec) {
        diff.tv_nsec = end.tv_nsec - start.tv_nsec;
        diff.tv_sec = end.tv_sec - start.tv_sec;
    }

    else {
        diff.tv_nsec = BIL - start.tv_nsec + end.tv_nsec;
        diff.tv_sec = end.tv_sec - start.tv_sec - 1;
    }

    return diff;
}

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
char* inputfile1,*inputfile2,*outputfile;
 
void *matrixMul(void *args) {
    pair *x = (pair *) args;
    // printf("thread %lld : st-%lld, ed-%lld\n", (x->st)/rpt, x->st, x->ed);

    keyB = ftok(".", 'B');
    //shmidB = shmget(keyB, dim3*dim2*sizeof(long long int), IPC_EXCL);
    matB = shmat(shmidB, 0, SHM_RDONLY);

    keyA = ftok(".", 'A');
    //shmidA = shmget(keyA, dim1*dim2*sizeof(long long int), IPC_EXCL);
    matA = shmat(shmidA, 0, SHM_RDONLY);

    
    // TO CHECK IF ROWS TO BE READ HAVE BEEN WRITTEN
    // If the last element of a row is -1, then you can't read the row, if it's positive, we can be certain that entire row is read
    int b=0;
    while(b==0) {
        b=1;
        for(long long int i = x->st; i < x->ed; i++) {
            if(matA[dim2*i+dim2-1]==-1)
                b=0;
        }
        // printf("in while loop\n");
    }
    // CALCULATIONS
    // printf("%lld\ndebug\n", matA[0]);
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

    // if (argc != 4) {
	// 	printf("Wrong Usage");
	// 	exit(-1);
	// }

    int num_threads = 10;

    dim1 = atoll(argv[1]);
    dim2 = atoll(argv[2]);
    dim3 = atoll(argv[3]);
    inputfile1 = argv[4];
	inputfile2 = argv[5];
	outputfile = argv[6];
    if(argc==9)
        num_threads = atoi(argv[8]);


    matC = (long long int *)malloc(sizeof(long long int)* dim1 * dim3);
    memset(matC, 0, dim1*dim3*sizeof(long long int));
    keyB = ftok(".", 'D');
    shmidB = shmget(keyB, (dim3*dim2+1000)*sizeof(long long int), IPC_EXCL);
    matB = shmat(shmidB, 0, SHM_RDONLY);
    // printf("hello %lld hello2 %lld \n", matB[10], matB[20]);

    keyA = ftok(".", 'C');
    shmidA = shmget(keyA, (dim1*dim2+1000)*sizeof(long long int), IPC_EXCL);
    matA = shmat(shmidA, 0, SHM_RDONLY);
    // printf("1\n");
    // printf("hello %lld hello2 %lld \n", matA[10], matA[20]);
    
    //shmctl(shmidA, IPC_RMID, NULL);
    //
    //shmctl(shmidB, IPC_RMID, NULL);


    struct timespec timestartP1,timeendP1;
    clock_gettime(CLOCK_REALTIME,&timestartP1);
    
    long long int b=0;
    while(b==0) {
        b=1;
        for(long long int i = 0; i < dim2; i++) {
            if(matB[dim2*i+dim2-1]==-1)
                b=0;
        }
    }

    // printf("AAAAAAAAAAAAAAAAAA\n");
    // for (long long int i = 0; i < dim1; i++) {
    //     for (long long int j = 0; j < dim2; j++) {
    //         //long long int k = matA[i*dim2+j];
    //         printf("%lld ", matA[i*dim2+j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // printf("BBBBBBBBBBBBBBBBBB\n");
    // for (long long int i = 0; i < dim2; i++) {
    //     for (long long int j = 0; j < dim3; j++) {
    //         printf("%lld ", matB[i*dim3+j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    

    //max threads : one thread for each row of C matrix.
    //NOTE: creating one thread for each element will be inefficient 
 
    //one thread for each column
 
    // printf("num thread p2 %d",num_threads);
    pthread_t threads[num_threads];
    if(num_threads>dim1){
        num_threads=dim1;
    }
 
    //num_threads = num_threads > dim1 ? dim1 : num_threads;
 
    rpt = (dim1 / num_threads) + 1; // rows per thread

    long long int ceil_rpt = dim1 / num_threads + 1; //4
    long long int flr_rpt = dim1 / num_threads; //3

    long long int rem = dim1 % num_threads; // 1

    //num threads = 30, dim1s = 100
    // 10 threads x 4 rows and 20 threads x 3 rows
 
    for(long long int l = 0 ; l<rem ; l++) { //
        //INITIALIZE ARRAY OF ARG BEFORE HAND?
        pair * arg = (pair *) malloc(sizeof(pair));
        arg->st = l * ceil_rpt;
        arg->ed = (l+1) * ceil_rpt;
        // printf("in main 1 : thread %lld : st-%lld, ed-%lld\n", l, arg->st, arg->ed);
        pthread_create(&threads[l], NULL, matrixMul, (void *) arg);
    }

    // printf("ceil %lld\n", rem*ceil_rpt);
    // printf("l lim %lld\n", num_threads-rem);
    for(long long int l = rem; l<num_threads; l++) {
        pair * arg = (pair *) malloc(sizeof(pair));
        arg->st = l * flr_rpt + rem * (ceil_rpt-flr_rpt);
        arg->ed = (l+1) * flr_rpt + rem * (ceil_rpt-flr_rpt);
        // printf("in main 2 : thread %lld : st-%lld, ed-%lld\n", l, arg->st, arg->ed);
        pthread_create(&threads[l], NULL, matrixMul, (void *) arg);
    }

 
    for (long long int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // for (long long int i = 0; i < dim1; i++) {
    //     for (long long int j = 0; j < dim3; j++)
    //         printf("%lld ", matC[i*dim3+j]);
    //     printf("\n");
    // }
 
    FILE *opfile;
    opfile = fopen(outputfile, "w");
    for (long long int i = 0; i < dim1; i++) {
        for (long long int j = 0; j < dim3; j++) {
            fprintf(opfile, "%lld ", matC[i*dim3+j]);
        }
        fputc('\n', opfile);
    }

    clock_gettime(CLOCK_REALTIME,&timeendP1);

    FILE*timefp2;

    timefp2 = fopen("datap2.csv","a+");
	long int time_elapsed = ((timeendP1.tv_sec * BIL + timeendP1.tv_nsec) - (timestartP1.tv_sec * BIL + timestartP1.tv_nsec));
		fprintf(timefp2, "%lld, %lld, %lld, %d, %ld\n", dim1, dim2, dim3, num_threads, time_elapsed);
    
    shmdt((void *) matA);
    shmdt((void *) matB);
    shmctl(shmidA, IPC_RMID, NULL);
    shmctl(shmidB, IPC_RMID, NULL);
	printf("P2 ends here\n");
    if(argc == 7){
     kill(getppid(), SIGUSR2);
    }
    return 0;
}