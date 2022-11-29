#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include<stdlib.h>

int main(){
    int shmid;
    long long int *matA;
    long long count = 20*50;
    long long int i = 0;
    key_t key = ftok(".", 'a');

    shmid = shmget(key, count*sizeof(long long int), IPC_EXCL);

    matA = shmat(shmid, 0, SHM_RDONLY);

    for(i=0; i<count; i++){
        printf("\n%lld---\n", matA[i] );
    }

    printf("\nRead to memory succesful--\n");

    shmdt((void *) matA);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}