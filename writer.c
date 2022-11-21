
//#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include<stdlib.h>


int main(){
   int *array;
    int count = 5;
    int i = 0;
    //int SizeMem;
    key_t key;
    int shmid;
 
    key = ftok(".", 'a');
    //SizeMem = sizeof(*array)*count;
    shmid = shmget(key, 8*sizeof(int), 0666 | IPC_CREAT);
    //printf("%d",key);
    array = (int *)shmat(shmid, NULL, 0);
    //array = malloc(sizeof(int)*count);
    for(i = 0; i < 5; i++){
        scanf("%d", &array[i]);
    }
    for(i = 0; i < count; i++){
        printf("\n%d---\n", array[i]);
    }
    printf("\nWriting to memory successful--\n");
    shmdt((void *) array);
    return 0;
}