#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>

int main() {

    // creating shared memory for process P1
    key_t keyP1 = ftok("OSASSGN2P1", 10);
    int shmidP1;
    
    if((shmidP1 = shmget(keyP1, 128, 0666 | IPC_CREAT)) == -1) {
        printf("\nshmget() error\n");
        exit(1);
    }
    
    char *shmPtrP1 = shmat(shmidP1, (void*)0, 0);
    
    if(shmPtrP1 == (char *) -1) {
        printf("\nshmPtr error\n");
        exit(2);
    }
    
    shmPtrP1[0] = '0';

    printf("SHM from P1: %c", shmPtrP1[0]);

    for(int i = 0; i < 5000; i++)
        printf("%d from P1\n", i);

    // sleep(10);
    
    shmPtrP1[0] = '1';

    return 0;
}