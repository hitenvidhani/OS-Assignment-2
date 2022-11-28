#include <stdio.h>
#include <string.h>
char* string;

int main(int argc,char *argv[]){
    // strcpy(string,argv[1]);
    string = argv[1];
    printf("%s",string);
    FILE*fp;
    fp = fopen(string,"r");
}