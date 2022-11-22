#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>

int main() {

    // creating shared memory for process P2    
    key_t keyP2 = ftok("OSASSGN2P2", 10);
    int shmidP2;
    
    if((shmidP2 = shmget(keyP2, 128, 0666 | IPC_CREAT)) == -1) {
        printf("\nshmget() error\n");
        exit(1);
    }
    
    char *shmPtrP2 = shmat(shmidP2, (void*)0, 0);
    
    if(shmPtrP2 == (char *) -1) {
        printf("\nshmPtr error\n");
        exit(2);
    }
    
    shmPtrP2[0] = '0';

    printf("SHM from P2: %c", shmPtrP2[0]);

    for(int i = 0; i < 5000; i++)
        printf("%d from P2\n", i);

    shmPtrP2[0] = '1';

    return 0;
}