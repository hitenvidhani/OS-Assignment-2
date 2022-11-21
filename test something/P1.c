#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    struct timeval start, end;
    gettimeofday(&start, NULL);

    int counter = 0;
    int pid = getpid();
    char *filename = "pid.txt";
    FILE *fp = fopen(filename, "w");
    
    fprintf(fp, "%d", pid);
    fclose(fp);
	
    while (1>0)
    {
        printf("%d \n", counter);
        if(counter>10000)
            counter-=10000;
        else
            counter++;
        sleep(2);
    }
    

    gettimeofday(&end, NULL);

    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
 
    printf("The elapsed time is %ld seconds and %ld micros\n", seconds, micros);

	return 0;
}
