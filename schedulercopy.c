#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <errno.h>

int P1_f = 0, P2_f = 0;

void sig_handler1(int signum) {
    P1_f = 1;
}

void sig_handler2(int signum) {
    P2_f = 1;
}

int main(int argc, char *argv[]) {

    signal(SIGUSR1, sig_handler1);
    signal(SIGUSR2, sig_handler2);

    // if(argc != 7) {
    //     printf("Usage: ./groupX_assignment2.out i j k in1.txt in2.txt out.txt");
    //     exit(-1);
    // }

    static struct timespec start1, start2, teststart;
    struct timespec curr;

    pid_t ch1 = fork();

    clock_gettime(CLOCK_REALTIME, &start1);

    if(ch1 == 0) {
        printf("started 1\n");
        if(execlp("./P1.out", "P1.out",argv[1],argv[2],argv[3], NULL) == -1) {
            perror("execlp() for P1 failed");
            return -1;
        }
    }

    if(ch1 > 0) {

        clock_gettime(CLOCK_REALTIME, &teststart);

        do {
            clock_gettime(CLOCK_REALTIME, &curr);
        }
        while(curr.tv_nsec - teststart.tv_nsec < 2000000);//9999999);

        kill(ch1, SIGSTOP);
        printf("stopped 1\n");
        // fflush(stdout);

        // clock_gettime(CLOCK_REALTIME, &teststart);

        // do {
        //     clock_gettime(CLOCK_REALTIME, &curr);
        // }
        // while(curr.tv_sec - teststart.tv_sec < 3);//2000000);
/*       
        // setting up reading shared memory for process P1
        key_t keyP1 = ftok("P1.c", 10);
        int shmidP1;

        if((shmidP1 = shmget(keyP1, 32, 0)) == -1) {
            printf("\nshmget1() error\n");
            printf("%d", errno);
            exit(1);
        }

        char *shmPtrP1 = shmat(shmidP1, (void*)0, 0);
        
        if(shmPtrP1 == (char *) -1) {
            printf("\nshmPtr1 error\n");
            exit(2);
        }
*/
/*
        printf("SHM after STOP before CONT: %c\n", shmPtrP1[0]);

        kill(ch1, SIGCONT);
        printf("conted\n");

        printf("SHM after CONT: %c\n", shmPtrP1[0]);
*/
        // do {
        //     clock_gettime(CLOCK_REALTIME, &curr);
        // }
        // while(curr.tv_nsec - start1.tv_nsec < 20);
        // // code to halt P1
        // kill(ch1, SIGSTOP);
        // printf("1 started: %llds %ldns\n", (long long)start1.tv_sec, start1.tv_nsec);
        // printf("1 stopped: %llds %ldns\n", (long long)curr.tv_sec, curr.tv_nsec);

        pid_t ch2 = fork();

        clock_gettime(CLOCK_REALTIME, &start2);

        if(ch2 == 0) {
            printf("starting P2\n");
            if(execlp("./P2.out", "P2.out",argv[1],argv[2],argv[3], NULL) == -1) {
                perror("execlp() for P1 failed");
                return -1;
            }
        }

        if(ch2 > 0) {
/*
            // setting up reading shared memory for process P2
            key_t keyP2 = ftok("P2.c", 10);
            int shmidP2;

            if((shmidP2 = shmget(keyP2, 32, 0)) == -1) {
                printf("\nshmget2() error\n");
                exit(1);
            }

            char *shmPtrP2 = shmat(shmidP2, (void*)0, 0);
            
            if(shmPtrP2 == (char *) -1) {
                printf("\nshmPtr2 error\n");
                exit(2);
            }
*/
            // clock_gettime(CLOCK_REALTIME, &teststart);

            // do {
            //     clock_gettime(CLOCK_REALTIME, &curr);
            // }
            // while(curr.tv_nsec - teststart.tv_nsec < 199999999);

            // kill(ch2, SIGSTOP);
            // printf("stopped 2, starting 1\n");

            // kill(ch1, SIGCONT);

            while(1) {   //shmPtrP1[0] == '0' | shmPtrP2[0] == '0'

                // printf("Ptr1: %c\n", shmPtrP1[0]);
                // printf("Ptr2: %c\n", shmPtrP2[0]);

                // if(shmPtrP1[0] == '1' && shmPtrP2[0] == '1')
                if(P1_f == 1 && P2_f == 1)
                    break;
                
                if(P2_f == 0) {
                //if(shmPtrP2[0] == '0') {
                    do {
                    clock_gettime(CLOCK_REALTIME, &curr);
                    }
                    while(curr.tv_nsec - start2.tv_nsec < 1999999);
                    // code to halt P2 and start P1 again
                    kill(ch2, SIGSTOP);
                    printf("Stopped 2\n");
                    // printf("2 started: %llds %ldns\n", (long long)start2.tv_sec, start2.tv_nsec);
                    // printf("2 stopped: %llds %ldns\n", (long long)curr.tv_sec, curr.tv_nsec);
                }

                if(P1_f == 0) {
                // if(shmPtrP1[0] == '0') {
                    clock_gettime(CLOCK_REALTIME, &start1);
                    kill(ch1, SIGCONT);
                    printf("Started 1\n");
                }
                
                // code to halt P1 and start P2 and cycle
                if(P1_f == 0){
                // if(shmPtrP1[0] == '0') {
                    do {
                    clock_gettime(CLOCK_REALTIME, &curr);
                    } while(curr.tv_nsec - start1.tv_nsec < 1999999); //2000000);//999999999
                    kill(ch1, SIGSTOP);
                    printf("Stopped 1\n");
                
                    // printf("1 started: %llds %ldns\n", (long long)start1.tv_sec, start1.tv_nsec);
                    // printf("1 stopped: %llds %ldns\n", (long long)curr.tv_sec, curr.tv_nsec);
                }

                if(P2_f == 0) {
                // if(shmPtrP2[0] == '0') {
                    clock_gettime(CLOCK_REALTIME, &start2);
                    kill(ch2, SIGCONT);
                    printf("Started 2\n");
                }
            }
            // printf("Ptr1: %c\n", shmPtrP1[0]);
            // printf("Ptr2: %c\n", shmPtrP2[0]);

            waitpid(ch1, NULL, 0);
            waitpid(ch2, NULL, 0);
            // printf("APtr1: %c\n", shmPtrP1[0]);
            // printf("APtr2: %c\n", shmPtrP2[0]);
        }
    // printf("SHM: %c", shmPtrP1[0]);
    // wait(NULL);
    }

    return 0;
}