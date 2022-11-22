#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main() {

/*
    // creating shared memory for process P1
    key_t keyP1 = ftok("P1.c", 10);
    int shmidP1;
    
    if((shmidP1 = shmget(keyP1, 32, 0666 | IPC_CREAT)) == -1) {
        printf("\nshmgetP1() error\n");
        exit(1);
    }
    
    char *shmPtrP1 = shmat(shmidP1, (void*)0, 0);
    
    if(shmPtrP1 == (char *) -1) {
        printf("\nshmPtrP1 error\n");
        exit(2);
    }
    
    shmPtrP1[0] = '0';
*/
    // printf("SHM from P1: %c", shmPtrP1[0]);

    for(int i = 0; i < 2500; i++)
        printf("%d from P1\n", i);
    
    kill(getppid(), SIGUSR1);

    // sleep(10);
    
    // shmPtrP1[0] = '1';

    return 0;
}