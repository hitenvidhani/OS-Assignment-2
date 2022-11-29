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

#define BIL 1000000000LL
#define MIL 1000000LL
long long int dim1;// = 20;
long long int dim2;// = 50;
long long int dim3;// = 20;
long long int *start_idxsA, *start_idxsB;
long long int *matA, *matB;
int num_threadsA = 8, num_threadsB = 8;
char* inputfile1,*inputfile2,*outputfile;
// B ka part
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
void *readMatrixRowB(void * args){
	long long int *x = (long long int *) args;
	//printf("%lld",*x);
	long long int num; char ch;
	FILE *fp;
	fp = fopen(inputfile2,"r");
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
	// printf("line %lld\n",j);
}
 
 
void get_start_idxsB(){
    start_idxsB=(long long int *)malloc(dim2*sizeof(long long int));
    start_idxsB[0]=0;
    FILE *fp;
    fp = fopen(inputfile2,"r");
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

void *readSecond(){

    int shmidB;
    key_t keyB = ftok(".", 'D');
    printf("%d\n",keyB);
    shmidB = shmget(keyB, (dim2*dim3+1000)*sizeof(long long int), 0666 | IPC_CREAT);
    printf("%dabc\n",shmidB);
    matB = (long long int *)shmat(shmidB, (void*)0, 0);
	memset(matB, -1, dim2*dim3*sizeof(long long int));
	printf("hello %lld\n", matB[0]);
    //shared mem end
	get_start_idxsB();
	printf("\n");
	pthread_t t[num_threadsB];
	long long int rpt = dim2/num_threadsB;
	clock_t time;
    time = clock();
	for(long long int i = 0; i < rpt; i++){
		for(long long int j=0;j<num_threadsB;j++){
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threadsB*i+j;
			pthread_create(&t[j],NULL,readMatrixRowB,arg);
		}
		for(long long int j =0; j < num_threadsB; j++){
			pthread_join(t[j],NULL);
		}
	}
	long long int left_over_rows = dim2%num_threadsB;
	
	for(long long int j=0;j<left_over_rows;j++){
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threadsB*rpt+j;
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
	fp = fopen(inputfile1,"r");
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
    fp = fopen(inputfile1,"r");
    long long int i = 1;
    char c=getc(fp);
    while(c!=EOF){
    	//printf("%c",getc(fp));
		// if(i==999)printf("%lld\n\n",start_idxsA[999]);
		// if(i==1000)printf("haha %lld\n",i);
    	while(c!='\n'){
   		//fseek(fp,1,SEEK_CUR);
            //printf("lolololo");
    		printf("%c",c);
    		c=getc(fp);
    	}
    	start_idxsA[i] = ftell(fp);
		// if(i==999)printf("%lld\n\n",start_idxsA[999]);
    	i++;
		// if(c==EOF || i==dim1)break;
    	printf("\n");
    	c = getc(fp);
   }
   printf("\n");
}

void *readFirst(){
 // hello
    int shmidA;
    key_t keyA = ftok(".", 'C');
    printf("%d\n",keyA);
    shmidA = shmget(keyA, (dim1*dim2+1000)*sizeof(long long int), 0666 | IPC_CREAT);
    printf("%dabc\n",shmidA);
    matA = (long long int *)shmat(shmidA, (void*)0, 0);
	memset(matA, -1, dim2*dim1*sizeof(long long int));
    //shared mem end
	get_start_idxsA();
	printf("\n");
	pthread_t t[num_threadsA];
	long long int rpt = dim1/num_threadsA;
	clock_t time;
    time = clock();
	for(long long int i = 0; i < rpt; i++){
		for(long long int j=0;j<num_threadsA;j++){
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threadsA*i+j;
			pthread_create(&t[j],NULL,readMatrixRowA,arg);
		}
		for(long long int j =0; j < num_threadsA; j++){
			pthread_join(t[j],NULL);
		}
	}
	long long int left_over_rows = dim1%num_threadsA;
	
	for(long long int j=0;j<left_over_rows;j++){
			long long int *arg = malloc(sizeof(long long int));
			*arg = num_threadsA*rpt+j;
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
    // if (argc != 4) {
	// 	printf("Wrong Usage");
	// 	exit(-1);
	// }
    dim1 = atoll(argv[1]);
    dim2 = atoll(argv[2]);
    dim3 = atoll(argv[3]);
	inputfile1 = argv[4];
	inputfile2 = argv[5];
	outputfile = argv[6]; 
	if(argc==9){
		num_threadsA = atoi(argv[7]);
		num_threadsB = atoi(argv[7]);
	}
	printf("num thread p1 %d",num_threadsA);
	//matA=(long long int *)malloc(dim1*dim2*sizeof(long long int));
    //shared memory stuff

	
	if(num_threadsA > dim1){
		num_threadsA = dim1;
	}

	if(num_threadsB > dim2){
		num_threadsB = dim2;
	}
	struct timespec timestartP1,timeendP1;
	clock_gettime(CLOCK_REALTIME,&timestartP1);
	pthread_t tr1,tr2;
	pthread_create(&tr2,NULL,readSecond,NULL);
	pthread_create(&tr1,NULL,readFirst,NULL);
	pthread_join(tr2,NULL);
	pthread_join(tr1,NULL);
	clock_gettime(CLOCK_REALTIME,&timeendP1);
	FILE*timefp;
	timefp = fopen("datap1.csv","a+");
	long int time_elapsed = ((timeendP1.tv_sec * BIL + timeendP1.tv_nsec) - (timestartP1.tv_sec * BIL + timestartP1.tv_nsec));
		fprintf(timefp, "%lld, %lld, %lld, %d, %ld\n", dim1, dim2, dim3, num_threadsA, time_elapsed);
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
	printf("P1 ends here\n");
	if(argc == 7){
		kill(getppid(), SIGUSR1);
	}
	return 0;
}