#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
// void sig_handler(int signum){
//   printf("Inside handler function\n");
// }

int main(){
    struct timespec t1,cur;

    
    
    pid_t ch = fork();
  if(ch==0){
    execlp("./temp.out","temp.out",NULL);
  }
  else{
    clock_gettime(CLOCK_REALTIME,&t1);
    do{
        clock_gettime(CLOCK_REALTIME,&cur);
    }while (cur.tv_nsec-t1.tv_nsec<9999999);
    kill(ch,SIGUSR1);
  }
  return 0;
}