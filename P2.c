#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//
long long int A [20001];
long long int B [60001];
long long int C [30001]={0};
long long int size_r = 20;
long long int size_c = 50;
 
int nArow, nAcol, nBrow, nBcol;
int rpt;

//      col*r + c;


//     int r = A / col;
//     int c = A % col;
 
typedef struct{
	int st,ed;
} pair;
 
void *matrixMul(void *args) {
    pair *x = (pair *) args;
    printf("thread %d : st-%d, ed-%d\n", (x->st)/rpt, x->st, x->ed);
    
    //TO CHECK IF ROWS TO BE READ HAVE BEEN WRITTEN
    //If the last element of a row is -1, then you can't read the row, if it's positive, we can be certain that entire row is read
    // int b=0;
    // while(b==0) {
    //     b=1;
    //     for(int i = x->st; i < x->ed; i++) {
    //         if(A[nAcol*i+nAcol-1]==-1)
    //             b=0;
    //     }
    //     printf("in while loop\n");
    // }
    //CALCULATIONS
    for (int i = x->st; i < x->ed; i++) {
        for (int j = 0; j < nBcol; j++) {
            for (int k = 0; k < nAcol; k++) {
                C[nBcol*i+j] += A[nAcol*i+k] * B[nBcol*k+j];
                // printf("A in i : %d,  j : %d , k : %d is : %lld\n", i, j, k,  A[nAcol*i+k]);
                // printf("B in i : %d,  j : %d , k : %d is : %lld\n", i, j, k,  B[nAcol*i+k]);
            }
        }
    }
 
}
 
int main(int argc, char *argv[]) {

    // int shmid;
    // long long int *matA;
    // long long count = size_r * size_c;
    // //long long int i = 0;
    // key_t key = ftok(".", 'c');

    // shmid = shmget(key, count*sizeof(long long int), IPC_EXCL);

    // matA = shmat(shmid, 0, SHM_RDONLY);
    // printf("hello %lld hello2 %lld \n", matA[0], matA[1]);
    // shmdt((void *) matA);
    // shmctl(shmid, IPC_RMID, NULL);


 
    struct timeval start, end;
    gettimeofday(&start, NULL);
 
    nArow = 20;
    nAcol = nBrow = 50;
    nBcol = 20;
 
    // for(int i=0;i<nAcol*nArow;i++)
    //     A[i]=i+1;
 
    // for(int i=0;i<nBcol*nBrow;i++)
    //     B[i]=i+1;
    long long int num;
    char ch;
    long long int j=0;
    FILE *inpfile1;
    inpfile1 = fopen("in2.txt", "r");
    j=0;
    for(int x=0;x<nBrow;x++) {
        j=0;
        while (fscanf(inpfile1, "%lld%c", &num, &ch)==2){
            //do something
			B[nBcol*x+j] = num;
            if(ch == '\n' | ch==EOF)
                break;
	    	j++;
        }
    }
    fclose(inpfile1);

    FILE *inpfile;
    inpfile = fopen("in1.txt", "r");
    
    for(int x=0;x<nArow;x++) {
        j=0;
        while (fscanf(inpfile, "%lld%c", &num, &ch)==2){
            //do something
			A[nAcol*(x)+j] = num;
            if(ch == '\n')
                break;
	    	j++;
        }
    }
    fclose(inpfile);
    
    printf("AAAAAAAAAAAAAAAAAA\n");
    for (int i = 0; i < nArow; i++) {
        for (int j = 0; j < nAcol; j++) {
            printf("%lld ", A[i*nAcol+j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("BBBBBBBBBBBBBBBBBB\n");
    for (int i = 0; i < nBrow; i++) {
        for (int j = 0; j < nBcol; j++) {
            printf("%lld ", B[i*nBcol+j]);
        }
        printf("\n");
    }
    printf("\n");
 
 
    //max threads : one thread for each row of C matrix.
    //NOTE: creating one thread for each element will be inefficient 
    pthread_t threads[100];
 
    //one thread for each column
 
    int num_threads = 3;

    if(num_threads>nArow){
        num_threads=nArow;
    }
 
    //num_threads = num_threads > nArow ? nArow : num_threads;
 
    rpt = (nArow / num_threads) + 1; // rows per thread

    long long int ceil_rpt = nArow / num_threads + 1; //4
    long long int flr_rpt = nArow / num_threads; //3

    long long int rem = nArow % num_threads; // 1

    //num threads = 2, nArows = 5
    // 10 threads x 4 rows and 20 threads x 3 rows
 
    for(int l = 0 ; l<rem ; l++) { //
        //INITIALIZE ARRAY OF ARG BEFORE HAND?
        pair * arg = (pair *) malloc(sizeof(pair));
        arg->st = l * ceil_rpt;
        arg->ed = (l+1) * ceil_rpt;
        printf("in main 1 : thread %d : st-%d, ed-%d\n", l, arg->st, arg->ed);
        pthread_create(&threads[l], NULL, matrixMul, (void *) arg);
    }

    printf("ceil %lld\n", rem*ceil_rpt);
    printf("l lim %lld\n", num_threads-rem);
    for(int l = rem; l<num_threads; l++) {
        pair * arg = (pair *) malloc(sizeof(pair));
        arg->st = l * flr_rpt + rem * (ceil_rpt-flr_rpt);
        arg->ed = (l+1) * flr_rpt + rem * (ceil_rpt-flr_rpt);
        printf("in main 2 : thread %d : st-%d, ed-%d\n", l, arg->st, arg->ed);
        pthread_create(&threads[l], NULL, matrixMul, (void *) arg);
    }

 
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    for (int i = 0; i < nArow; i++) {
        for (int j = 0; j < nBcol; j++)
            printf("%lld ", C[i*nBcol+j]);
        printf("\n");
    }
 
    FILE *opfile;
    opfile = fopen("output.txt", "w");
    for (int i = 0; i < nArow; i++) {
        for (int j = 0; j < nBcol; j++) {
            fprintf(opfile, "%lld ", C[i*nBcol+j]);
        }
        fputc('\n', opfile);
    }
 
    gettimeofday(&end, NULL);
 
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
 
    printf("The elapsed time is %ld seconds and %ld micros\n", seconds, micros);
 
	return 0;
}