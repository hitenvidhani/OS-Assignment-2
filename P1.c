#include <stdio.h>
#include<pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<string.h>
 
#define num_threads 1
 
long long int dim1;// = 20;
long long int dim2;// = 50;
long long int dim3;// = 20;
long long int *start_idxsA, *start_idxsB;
long long int *matA, *matB;
// B ka part

void *readMatrixRowB(void * args){
	long long int *x = (long long int *) args;
	//printf("%lld",*x);
	long long int num; char ch;
	FILE *fp;
	fp = fopen("in2.txt","r");
	fseek(fp,start_idxsB[*x],SEEK_CUR);
	long long int j = 0;
	while (fscanf(fp, "%lld%c", &num, &ch)==2){
            //do something
			matB[dim3*(*x)+j] = num;
			//printf("%lld ",num);
            if(ch == '\n')
                break;
			j++;
    }
}
 
 
void get_start_idxsB(){
    start_idxsB=(long long int *)malloc(dim2*sizeof(long long int));
    start_idxsB[0]=0;
    FILE *fp;
    fp = fopen("in2.txt","r");
    long long int i = 1;
    char c=getc(fp);
    while(c!=EOF){
    	//printf("%c",getc(fp));
    	while(c!='\n'){
   		//fseek(fp,1,SEEK_CUR);
            //printf("lolololo");
    		printf("%c",c);
    		c=getc(fp);
    	}
    	//if(c==EOF)break;
    	start_idxsB[i] = ftell(fp);
    	i++;
    	printf("\n");
    	c = getc(fp);
   }
   printf("\n");
}

void readSecond(){

    int shmidB;
    key_t keyB = ftok(".", 'B');
    printf("%d\n",keyB);
    shmidB = shmget(keyB, dim2*dim3*sizeof(long long int), 0666 | IPC_CREAT);
    //printf("%dabc\n",shmidB);
    matB = (long long int *)shmat(shmidB, (void*)0, 0);
	memset(matB, -1, dim2*dim3*sizeof(long long int));
	printf("hello %lld\n", matB[0]);
    //shared mem end
	get_start_idxsB();
	printf("\n");
	pthread_t t[num_threads];
	long long int rpt = dim2/num_threads;
	clock_t time;
    time = clock();
	for(long long int i = 0; i < rpt; i++){
		for(long long int j=0;j<num_threads;j++){
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threads*i+j;
			pthread_create(&t[j],NULL,readMatrixRowB,arg);
		}
		for(long long int j =0; j < num_threads; j++){
			pthread_join(t[j],NULL);
		}
	}
	long long int left_over_rows = dim2%num_threads;
	
	for(long long int j=0;j<left_over_rows;j++){
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threads*rpt+j;
			pthread_create(&t[j],NULL,readMatrixRowB,arg);
            //check here if no any work
	}
	for(long long int j =0; j < left_over_rows; j++){
			pthread_join(t[j],NULL);
	}
	time = clock() - time;
	double time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds
 
    printf("took %f seconds to execute \n", time_taken);
	printf("\n");
	//for(long long int i = 0; i < dim1; i++){
	//	printf("%lld ",start_idxsA[i]);
	//}
	FILE *fp;
	fp = fopen("outputB.txt","w");
	for(long long int i = 0; i < dim2; i++){
		for(long long int j = 0; j < dim3; j++){
			fprintf(fp,"%lld ",matB[i*dim3+j]);
		}
		fputc('\n',fp);
	}

    
    shmdt((void *) matB);

}

// B ka part end
 
void *readMatrixRowA(void * args){
	long long int *x = (long long int *) args;
	//printf("%lld",*x);
	long long int num; char ch;
	FILE *fp;
	fp = fopen("in1.txt","r");
	fseek(fp,start_idxsA[*x],SEEK_CUR);
	long long int j = 0;
	//printf("helllo %lld", matA[0]);
	while (fscanf(fp, "%lld%c", &num, &ch)==2){
            //do something
			matA[dim2*(*x)+j] = num;
			//printf("%lld ",num);
            if(ch == '\n')
                break;
			j++;
    }
}
 
 
void get_start_idxsA(){
    start_idxsA=(long long int *)malloc(dim1*sizeof(long long int));
    start_idxsA[0]=0;
    FILE *fp;
    fp = fopen("in1.txt","r");
    long long int i = 1;
    char c=getc(fp);
    while(c!=EOF){
    	//printf("%c",getc(fp));
    	while(c!='\n'){
   		//fseek(fp,1,SEEK_CUR);
            //printf("lolololo");
    		printf("%c",c);
    		c=getc(fp);
    	}
    	//if(c==EOF)break;
    	start_idxsA[i] = ftell(fp);
    	i++;
    	printf("\n");
    	c = getc(fp);
   }
   printf("\n");
}

void readFirst(){
 // hello
    int shmidA;
    key_t keyA = ftok(".", 'A');
    printf("%d\n",keyA);
    shmidA = shmget(keyA, dim1*dim2*sizeof(long long int), 0666 | IPC_CREAT);
    printf("%dabc\n",shmidA);
    matA = (long long int *)shmat(shmidA, (void*)0, 0);
	memset(matA, -1, dim2*dim1*sizeof(long long int));
    //shared mem end
	get_start_idxsA();
	printf("\n");
	pthread_t t[num_threads];
	long long int rpt = dim1/num_threads;
	clock_t time;
    time = clock();
	for(long long int i = 0; i < rpt; i++){
		for(long long int j=0;j<num_threads;j++){
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threads*i+j;
			pthread_create(&t[j],NULL,readMatrixRowA,arg);
		}
		for(long long int j =0; j < num_threads; j++){
			pthread_join(t[j],NULL);
		}
	}
	long long int left_over_rows = dim1%num_threads;
	
	for(long long int j=0;j<left_over_rows;j++){
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threads*rpt+j;
			pthread_create(&t[j],NULL,readMatrixRowA,arg);
	}
	for(long long int j =0; j < left_over_rows; j++){
			pthread_join(t[j],NULL);
	}
	time = clock() - time;
	double time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds
 
    printf("took %f seconds to execute \n", time_taken);
	printf("\n");
	//for(long long int i = 0; i < dim1; i++){
	//	printf("%lld ",start_idxsA[i]);
	//}
	FILE *fp;
	fp = fopen("outputA.txt","w");
	for(long long int i = 0; i < dim1; i++){
		for(long long int j = 0; j < dim2; j++){
			fprintf(fp,"%lld ",matA[i*dim2+j]);
		}
		fputc('\n',fp);
	}

    
    shmdt((void *) matA);

}
 
 
int main(int argc,char *argv[])
{
    if (argc != 4) {
		printf("Wrong Usage");
		exit(-1);
	}
    dim1 = atoi(argv[1]);
    dim2 = atoi(argv[2]);
    dim3 = atoi(argv[3]);
	//matA=(long long int *)malloc(dim1*dim2*sizeof(long long int));
    //shared memory stuff

    readSecond();
    readFirst();
    
    // int shmid;
    // key_t key = ftok(".", 'c');
    // printf("%d\n",key);
    // shmid = shmget(key, dim1*dim2*sizeof(long long int), 0666 | IPC_CREAT);
    // printf("%dabc\n",shmid);
    // matA = (long long int *)shmat(shmid, (void*)0, 0);
    // //shared mem end
	// get_start_idxsA();
	// printf("\n");
	// pthread_t t[num_threads];
	// long long int rpt = dim1/num_threads;
	// clock_t time;
    // time = clock();
	// for(long long int i = 0; i < rpt; i++){
	// 	for(long long int j=0;j<num_threads;j++){
	// 		long long int *arg = malloc(sizeof(long long int));
	// 		*arg = num_threads*i+j;
	// 		pthread_create(&t[j],NULL,readMatrixRow,arg);
	// 	}
	// 	for(long long int j =0; j < num_threads; j++){
	// 		pthread_join(t[j],NULL);
	// 	}
	// }
	// long long int left_over_rows = dim1%num_threads;
	
	// for(long long int j=0;j<left_over_rows;j++){
	// 		long long int *arg = malloc(sizeof(long long int));
	// 		*arg = num_threads*rpt+j;
	// 		pthread_create(&t[j],NULL,readMatrixRow,arg);
	// }
	// for(long long int j =0; j < left_over_rows; j++){
	// 		pthread_join(t[j],NULL);
	// }
	// time = clock() - time;
	// double time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds
 
    // printf("took %f seconds to execute \n", time_taken);
	// printf("\n");
	// //for(long long int i = 0; i < dim1; i++){
	// //	printf("%lld ",start_idxsA[i]);
	// //}
	// FILE *fp;
	// fp = fopen("output.txt","w");
	// for(long long int i = 0; i < dim1; i++){
	// 	for(long long int j = 0; j < dim2; j++){
	// 		fprintf(fp,"%lld ",matA[i*dim2+j]);
	// 	}
	// 	fputc('\n',fp);
	// }

    
    // shmdt((void *) matA);
	return 0;
}