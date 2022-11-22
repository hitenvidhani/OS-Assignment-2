#include<stdio.h>
#include<signal.h>

int t=0;
void sig_handler(int signum){
    t=1;
  printf("Inside handler function\n");
}

int main(){
    signal(SIGUSR1,sig_handler);
    while(1){
        printf("%d\n",t);
        if(t==1){
            printf("rec sig");
            break;
        }
    }
   // Register signal handler
//   printf("Inside main function\n");
//   raise(SIGUSR1);
//   printf("Inside main function\n");
  return 0;
}