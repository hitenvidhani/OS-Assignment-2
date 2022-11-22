#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    // if(argc != 7) {
    //     printf("Usage: ./groupX_assignment2.out i j k in1.txt in2.txt out.txt");
    //     exit(-1);
    // }

    static struct timespec start1, start2;
    struct timespec curr;

    // setting up reading shared memory for process P1
    key_t keyP1 = ftok("OSASSGN2P1", 10);
    int shmidP1;

    if((shmidP1 = shmget(keyP1, 128, 0)) == -1) {
        printf("\nshmget1() error\n");
        exit(1);
    }

    char *shmPtrP1 = shmat(shmidP1, (void*)0, 0);
    
    if(shmPtrP1 == (char *) -1) {
        printf("\nshmPtr error\n");
        exit(2);
    }

    // setting up reading shared memory for process P2
    key_t keyP2 = ftok("OSASSGN2P2", 10);
    int shmidP2;

    if((shmidP2 = shmget(keyP2, 128, 0)) == -1) {
        printf("\nshmget2() error\n");
        exit(1);
    }

    char *shmPtrP2 = shmat(shmidP2, (void*)0, 0);
    
    if(shmPtrP2 == (char *) -1) {
        printf("\nshmPtr error\n");
        exit(2);
    }

    pid_t ch1 = fork();

    clock_gettime(CLOCK_REALTIME, &start1);

    if(ch1 == 0) {
        if(execlp("./P1.out", "P1.out", NULL) == -1) {
            perror("execlp() for P1 failed");
            return -1;
        }
    }

    else if(ch1 > 0) {

        do {
            clock_gettime(CLOCK_REALTIME, &curr);
        }
        while(curr.tv_nsec - start1.tv_nsec < 200000);
        // code to halt P1
        kill(ch1, SIGSTOP);
        printf("1 started: %llds %ldns\n", (long long)start1.tv_sec, start1.tv_nsec);
        printf("1 stopped: %llds %ldns\n", (long long)curr.tv_sec, curr.tv_nsec);

        pid_t ch2 = fork();

        clock_gettime(CLOCK_REALTIME, &start2);

        if(ch2 == 0) {
            if(execlp("./P2.out", "P2.out", NULL) == -1) {
                perror("execlp() for P2 failed");
                return -1;
            }
        }

        else if(ch2 > 0) {

            while(shmPtrP1[0] == '0' | shmPtrP2[0] == '0') {   //shmPtrP1[0] == '0' | shmPtrP2[0] == '0'

                printf("Ptr1: %c\n", shmPtrP1[0]);
                printf("Ptr2: %c\n", shmPtrP2[0]);

                do {
                    clock_gettime(CLOCK_REALTIME, &curr);
                } while(curr.tv_nsec - start2.tv_nsec < 200000);
                // code to halt P2 and start P1 again
                kill(ch2, SIGSTOP);
                
                printf("2 started: %llds %ldns\n", (long long)start2.tv_sec, start2.tv_nsec);
                printf("2 stopped: %llds %ldns\n", (long long)curr.tv_sec, curr.tv_nsec);
                
                clock_gettime(CLOCK_REALTIME, &start1);
                kill(ch1, SIGCONT);
                
                // loop to halt P1 and start P2 and cycle
                do {
                    clock_gettime(CLOCK_REALTIME, &curr);
                } while(curr.tv_nsec - start1.tv_nsec < 200000);
                kill(ch1, SIGSTOP);
                
                printf("1 started: %llds %ldns\n", (long long)start1.tv_sec, start1.tv_nsec);
                printf("1 stopped: %llds %ldns\n", (long long)curr.tv_sec, curr.tv_nsec);

                clock_gettime(CLOCK_REALTIME, &start2);
                kill(ch2, SIGCONT);
            }

            waitpid(ch1, NULL, 0);
            waitpid(ch2, NULL, 0);
        }
    }

    return 0;
}
