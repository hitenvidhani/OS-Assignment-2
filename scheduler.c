#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define BIL 1000000000LL
#define MIL 1000000LL

int P1_f = 0, P2_f = 0;
long long wait1_s = 0, wait2_s = 0, run1_s = 0, run2_s = 0;
long wait1_ns = 0, wait2_ns = 0, run1_ns = 0, run2_ns = 0;

struct timespec S1, S2, E1, E2, W1S, W1E, W2S, W2E;
int num_r_P1 = 0, num_r_P2 = 0, num_cs = 0;

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

void sig_handler1(int signum) {
    P1_f = 1;
    clock_gettime(CLOCK_REALTIME, &E1); //for TAT1
}

void sig_handler2(int signum) {
    P2_f = 1;
    clock_gettime(CLOCK_REALTIME, &E2); //for TAT2
}

int main(int argc, char *argv[]) {


   // shmctl(shmidA, IPC_RMID, NULL);
   // shmctl(shmidB, IPC_RMID, NULL);
    signal(SIGUSR1, sig_handler1);
    signal(SIGUSR2, sig_handler2);

    // if(argc != 7) {
    //     printf("Usage: ./groupX_assignment2.out i j k in1.txt in2.txt out.txt");
    //     exit(-1);
    // }

    int idA, idB;
    key_t keyA, keyB;
    int dim1, dim2, dim3;

    dim1 = atoi(argv[1]);
    dim2 = atoi(argv[2]);
    dim3 = atoi(argv[3]);

    keyB = ftok(".", 'D');
    // printf("%d\n",keyB);
    idB = shmget(keyB, (dim2*dim3)*sizeof(long long int), 0666 | IPC_CREAT);
    // printf("%dabc\n",shmidB);
    // matB = (long long int *)shmat(shmidB, (void*)0, 0);
	// memset(matB, -1, dim2*dim3*sizeof(long long int));

    keyA = ftok(".", 'C');
    // printf("%d\n",keyA);
    idA = shmget(keyA, (dim1*dim2)*sizeof(long long int), 0666 | IPC_CREAT);
    // printf("%dabc\n",shmidA);
    // matA = (long long int *)shmat(shmidA, (void*)0, 0);
	// memset(matA, -1, dim2*dim1*sizeof(long long int));

    struct timespec start1, start2, curr, F, teststart;

    int time_quant = 1 * MIL;

    pid_t ch1 = fork();

    clock_gettime(CLOCK_REALTIME, &start1);
    clock_gettime(CLOCK_REALTIME, &S1);
    num_r_P1++;

    if(ch1 == 0) {
        printf("Starting P1\n");
        // if(argc == 7) {
            if(execlp("./P1.out", "P1.out", argv[1], argv[2], argv[3],argv[4], argv[5], argv[6], NULL) == -1) {
                perror("execlp() for P1 failed");
                return -1;
            }
        // }
        // else if(argc == 9) {
        //      if(execlp("./P1.out", "P1.out", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], NULL) == -1) {
        //         perror("execlp() for P1 failed");
        //         return -1;
        //     }
        // }
    }

    if(ch1 > 0) {

        // clock_gettime(CLOCK_REALTIME, &teststart);

        // if(P1_f == 0) {
            do {
                clock_gettime(CLOCK_REALTIME, &curr);
            }
            while(curr.tv_sec - start1.tv_sec == 0 && curr.tv_nsec - start1.tv_nsec < time_quant);//99999999);//2000000);//9999999);
            kill(ch1, SIGSTOP);

            run1_s += calcTimeDiff(curr, start1).tv_sec;//(curr.tv_sec - start1.tv_sec);
            run1_ns += calcTimeDiff(curr, start1).tv_nsec;//(curr.tv_nsec - start1.tv_nsec);
            
            clock_gettime(CLOCK_REALTIME, &W1S);
            
            num_cs++;
            
            printf("Stopped P1\n");
        // }

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
        clock_gettime(CLOCK_REALTIME, &S2);
        num_r_P2++;

        clock_gettime(CLOCK_REALTIME, &teststart);

        // do {
        //     clock_gettime(CLOCK_REALTIME, &curr);
        // }
        // while(curr.tv_sec - teststart.tv_sec < 10);//2000000);

        if(ch2 == 0) {
            printf("Starting P2\n");
            // if(argc == 7) {
               if(execlp("./P2.out", "P2.out", argv[1], argv[2], argv[3],argv[4], argv[5], argv[6], NULL) == -1) {
                    perror("execlp() for P2 failed");
                    return -1;
                }     
            // }
            // else if(argc == 9) {
            //     if(execlp("./P2.out", "P2.out", argv[1], argv[2], argv[3],argv[4], argv[5], argv[6],argv[7],argv[8], NULL) == -1) {
            //         perror("execlp() for P2 failed");
            //         return -1;
            //     }
            // }
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
/*
            while(1) {   //shmPtrP1[0] == '0' | shmPtrP2[0] == '0' //1 //waitpid(ch1, NULL, 0) != ch1 || waitpid(ch2, NULL, 0) != ch2
                // printf("Ptr1: %c\n", shmPtrP1[0]);
                // printf("Ptr2: %c\n", shmPtrP2[0]);
                // if(shmPtrP1[0] == '1' && shmPtrP2[0] == '1')
                if(P1_f == 1 && P2_f == 1) //if both P1 and P2 have ended
                // if(waitpid(ch1, NULL, 0) == ch1 && waitpid(ch2, NULL, 0) == ch2)
                    break;
                
                // if(waitpid(ch2, NULL, 0) != ch2) {
                if(P2_f == 0) {
                //if(shmPtrP2[0] == '0') {
                    do {
                    clock_gettime(CLOCK_REALTIME, &curr);
                    }
                    while(curr.tv_nsec - start2.tv_nsec < 1999999);
                    // code to halt P2 and start P1 again
                    kill(ch2, SIGSTOP);
                    // printf("2 started: %llds %ldns\n", (long long)start2.tv_sec, start2.tv_nsec);
                    // printf("2 stopped: %llds %ldns\n", (long long)curr.tv_sec, curr.tv_nsec);
                }
                // if(waitpid(ch1, NULL, 0) != ch1) {
                if(P1_f == 0) {
                // if(shmPtrP1[0] == '0') {
                    clock_gettime(CLOCK_REALTIME, &start1);
                    kill(ch1, SIGCONT);
                }
                
                // code to halt P1 and start P2 and cycle
                // if(waitpid(ch1, NULL, 0) != ch1) {
                if(P1_f == 0){
                // if(shmPtrP1[0] == '0') {
                    do {
                    clock_gettime(CLOCK_REALTIME, &curr);
                    } while(curr.tv_nsec - start1.tv_nsec < 1999999); //2000000);//999999999
                    kill(ch1, SIGSTOP);
                
                    // printf("1 started: %llds %ldns\n", (long long)start1.tv_sec, start1.tv_nsec);
                    // printf("1 stopped: %llds %ldns\n", (long long)curr.tv_sec, curr.tv_nsec);
                }
                // if(waitpid(ch2, NULL, 0) != ch2) {
                if(P2_f == 0) {
                // if(shmPtrP2[0] == '0') {
                    clock_gettime(CLOCK_REALTIME, &start2);
                    kill(ch2, SIGCONT);
                }
            }
            */
            // if(P2_f == 0) {
                do {
                    clock_gettime(CLOCK_REALTIME, &curr);
                } while(curr.tv_sec - start2.tv_sec == 0 && curr.tv_nsec - start2.tv_nsec < time_quant);
                        // code to halt P2 and start P1 again
                kill(ch2, SIGSTOP);

                run2_s += calcTimeDiff(curr, start2).tv_sec; //(curr.tv_sec - start2.tv_sec);
                run2_ns += calcTimeDiff(curr, start2).tv_nsec; //(curr.tv_nsec - start2.tv_nsec);

                clock_gettime(CLOCK_REALTIME, &W2S);
                
                num_cs++;
                
                printf("Stopped P2\n");

                kill(ch1, SIGSTOP);
            // }

            while(P1_f == 0 || P2_f == 0) {
            // while(waitpid(ch1, NULL, WNOHANG) == 0 && waitpid(ch2, NULL, WNOHANG) == 0) {   //shmPtrP1[0] == '0' | shmPtrP2[0] == '0' //1 //waitpid(ch1, NULL, 0) != ch1 || waitpid(ch2, NULL, 0) != ch2

                // printf("Ptr1: %c\n", shmPtrP1[0]);
                // printf("Ptr2: %c\n", shmPtrP2[0]);

                // if(shmPtrP1[0] == '1' && shmPtrP2[0] == '1')
                // if(P1_f == 1 && P2_f == 1) //if both P1 and P2 have ended
                // if(waitpid(ch1, NULL, WNOHANG) == ch1 && waitpid(ch2, NULL, WNOHANG) == ch2)
                    // break;
/*
                // if(waitpid(ch2, NULL, WNOHANG) != ch2) {
                // if(P2_f == 0) {
                //if(shmPtrP2[0] == '0') {
                    do {
                    clock_gettime(CLOCK_REALTIME, &curr);
                    }
                    while(curr.tv_nsec - start2.tv_nsec < 1999999);
                    // code to halt P2 and start P1 again
                    kill(ch2, SIGSTOP);
                    // printf("2 started: %llds %ldns\n", (long long)start2.tv_sec, start2.tv_nsec);
                    // printf("2 stopped: %llds %ldns\n", (long long)curr.tv_sec, curr.tv_nsec);
                // }
*/
                // if(waitpid(ch1, NULL, WNOHANG) != ch1) {
                if(P1_f == 0) {
                // if(shmPtrP1[0] == '0') {
                    clock_gettime(CLOCK_REALTIME, &start1);
                    printf("Start 1\n");
                    
                    num_r_P1++;
                    
                    kill(ch1, SIGCONT);
     
                    clock_gettime(CLOCK_REALTIME, &W1E);
                    
                    wait1_s += calcTimeDiff(W1E, W1S).tv_sec; //(W1E.tv_sec - W1S.tv_sec);
                    wait1_ns += calcTimeDiff(W1E, W1S).tv_nsec; //(W1E.tv_nsec - W1S.tv_nsec);
                }
                
                // code to halt P1 and start P2 and cycle
                // if(waitpid(ch1, NULL, WNOHANG) != ch1) {
                if(P1_f == 0) {
                // if(shmPtrP1[0] == '0') {
                    do {
                        clock_gettime(CLOCK_REALTIME, &curr);
                        // printf("%ld : %ld\n", curr.tv_sec - start1.tv_sec, curr.tv_nsec - start1.tv_nsec);
                    } while(curr.tv_sec - start1.tv_sec == 0 && curr.tv_nsec - start1.tv_nsec < time_quant);//< 1999999); //2000000);//999999999
                    kill(ch1, SIGSTOP);
                    clock_gettime(CLOCK_REALTIME, &W1S);

                    run1_s += calcTimeDiff(curr, start1).tv_sec; //(curr.tv_sec - start1.tv_sec);
                    run1_ns += calcTimeDiff(curr, start1).tv_nsec; //(curr.tv_nsec - start1.tv_nsec);

                    num_cs++;
                    printf("Stop 1\n");
                
                    // printf("1 started: %llds %ldns\n", (long long)start1.tv_sec, start1.tv_nsec);
                    // printf("1 stopped: %llds %ldns\n", (long long)curr.tv_sec, curr.tv_nsec);
                }

                // if(waitpid(ch2, NULL, WNOHANG) != ch2) {
                if(P2_f == 0) {
                // if(shmPtrP2[0] == '0') {
                    clock_gettime(CLOCK_REALTIME, &start2);
                    printf("Start 2\n");
                    
                    num_r_P2++;

                    kill(ch2, SIGCONT);

                    clock_gettime(CLOCK_REALTIME, &W2E);
                    
                    wait2_s += calcTimeDiff(W2E, W2S).tv_sec; //(W2E.tv_sec - W2S.tv_sec);
                    wait2_ns += calcTimeDiff(W2E, W2S).tv_nsec; //(W2E.tv_nsec - W2S.tv_nsec);
                }

                if(P2_f == 0) {

                    do {
                        clock_gettime(CLOCK_REALTIME, &curr);
                    } while(curr.tv_sec - start2.tv_sec == 0 && curr.tv_nsec - start2.tv_nsec < time_quant);
                    // code to halt P2 and start P1 again

                    kill(ch2, SIGSTOP);
                    clock_gettime(CLOCK_REALTIME, &W2S);

                    run2_s += calcTimeDiff(curr, start2).tv_sec; //(curr.tv_sec - start2.tv_sec);
                    run2_ns += calcTimeDiff(curr, start2).tv_nsec; //(curr.tv_nsec - start2.tv_nsec);
                    
                    num_cs++;
                    printf("Stop 2\n");
                }
            }

            clock_gettime(CLOCK_REALTIME, &F);
            // while(wait(NULL) > 0);
            // wait(NULL);
            // wait(NULL);
/*
            if(waitpid(ch1, NULL, WNOHANG) != 0) {
                kill(ch2, SIGCONT);
                // waitpid(ch2, NULL, 0);
                wait(NULL);
            }
            if(waitpid(ch2, NULL, WNOHANG) != 0) {
                kill(ch1, SIGCONT);
                // waitpid(ch1, NULL, 0);
                wait(NULL);
            }
*/
            // printf("Ptr1: %c\n", shmPtrP1[0]);
            // printf("Ptr2: %c\n", shmPtrP2[0]);

            // waitpid(ch1, NULL, 0);
            // waitpid(ch2, NULL, 0);
            // printf("APtr1: %c\n", shmPtrP1[0]);
            // printf("APtr2: %c\n", shmPtrP2[0]);

            wait1_s += (wait1_ns / BIL);
            wait1_ns = wait1_ns % BIL;

            wait2_s += (wait2_ns / BIL);
            wait2_ns = wait2_ns % BIL;

            run1_s += (run1_ns / BIL);
            run1_ns = run1_ns % BIL;

            run2_s += (run2_ns / BIL);
            run2_ns = run2_ns % BIL;

            long long int tot_time_s = calcTimeDiff(F, S1).tv_sec; //F.tv_sec - S1.tv_sec;
            long int tot_time_ns = calcTimeDiff(F, S1).tv_nsec; //F.tv_nsec - S1.tv_nsec;

            long long int TAT1_s = calcTimeDiff(E1, S1).tv_sec; //E1.tv_sec - S1.tv_sec;
            long int TAT1_ns = calcTimeDiff(E1, S1).tv_nsec; //E1.tv_nsec - S1.tv_nsec;
            
            long long int TAT2_s = calcTimeDiff(E2, S2).tv_sec; //E2.tv_sec - S2.tv_sec;
            long int TAT2_ns = calcTimeDiff(E2, S2).tv_nsec; //E2.tv_nsec - S2.tv_nsec;

            // long long int run_wait1_s = run1_s + wait1_s;
            // long int run_wait1_ns = run1_ns + wait1_ns;

            // run_wait1_s += (run_wait1_ns / BIL);
            // run_wait1_ns = run_wait1_ns % BIL;

            // long long int run_wait2_s = run2_s + wait2_s;
            // long int run_wait2_ns = run2_ns + wait2_ns;

            // run_wait2_s += (run_wait2_ns / BIL);
            // run_wait2_ns = run_wait2_ns % BIL;

            long long int run12_s = run1_s + run2_s;
            long int run12_ns = run1_ns + run2_ns;

            run12_s += (run12_ns / BIL);
            run12_ns = run12_ns % BIL;

            long long int wait12_s = wait1_s + wait2_s;
            long int wait12_ns = wait1_ns + wait2_ns;

            wait12_s += (wait12_ns / BIL);
            wait12_ns = wait12_ns % BIL;

            long long int tot_run_wait_s = run12_s + wait12_s;
            long int tot_run_wait_ns = run12_ns + wait12_ns;

            tot_run_wait_s += (tot_run_wait_ns / BIL);
            tot_run_wait_ns = tot_run_wait_ns % BIL;

            // long long int switch_overhead_s = calcTimeDiff()
            
            printf("Total Time: %llds %ldns\n", tot_time_s, tot_time_ns);
            
            printf("TAT1: %llds %ldns\n", TAT1_s, TAT1_ns);
            printf("TAT2: %llds %ldns\n", TAT2_s, TAT2_ns);

            printf("Waiting Time 1: %llds %ldns\n", wait1_s, wait1_ns);
            printf("Waiting Time 2: %llds %ldns\n", wait2_s, wait2_ns);

            printf("Run Time 1: %llds %ldns\n", run1_s, run1_ns);
            printf("Run Time 2: %llds %ldns\n", run2_s, run2_ns);

            printf("No. of times P1 got turn: %d\n", num_r_P1);
            printf("No. of times P2 got turn: %d\n", num_r_P2);
            
            printf("No. of Context Switches: %d\n", num_cs);

            long long int totTime, TAT1, TAT2, totTAT, wait12, run12, switch_ovhd;

            totTime = ((F.tv_sec * BIL + F.tv_nsec) - (S1.tv_sec * BIL + S1.tv_nsec));
            TAT1 = ((E1.tv_sec * BIL + E1.tv_nsec) - (S1.tv_sec * BIL + S1.tv_nsec));
            TAT2 = ((E2.tv_sec * BIL + E2.tv_nsec) - (S2.tv_sec * BIL + S2.tv_nsec));
            totTAT = TAT1 + TAT2;
            wait12 = wait12_ns + (wait12_s * BIL);
            run12 = run12_ns + (run12_s * BIL);
            switch_ovhd = totTime - run12;

            if(argc == 7) {
                //FILE *fp = fopen("datasched.csv", "a+");

                //fprintf(fp, "%lld, %lld, %lld, %lld, %lld, %lld\n", atoll(argv[1]), atoll(argv[2]), atoll(argv[3]), totTAT, wait12, switch_ovhd);
                //fclose(fp);
            }

            else if(argc == 9) {
                //FILE *fp = fopen("datasched.csv", "a+");

                //fprintf(fp, "%lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld\n", atoll(argv[1]), atoll(argv[2]), atoll(argv[3]), atoll(argv[7]), atoll(argv[8]), totTAT, wait12, switch_ovhd);
                //fclose(fp);
            }
            // int wait_time1_s = ;
            // int wait_time1_ns = ;

            // int wait_time2_s = ;
            // int wait_time2_ns = ;

            // int run_time1_s = ;
            // int run_time1_ns = ;

            // int run_time2_s = ;
            // int run_time2_ns = ;

        }
    // printf("SHM: %c", shmPtrP1[0]);
    // wait(NULL);
    // shmctl(idA, IPC_RMID, NULL);
    // shmctl(idB, IPC_RMID, NULL);
    }

    return 0;
}
