#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



#include "parse.h" 
#include "avl_tree.h"
#include "exec_job.h"


#define MAX_LINE_LEN 1024

struct Node * fg_procs;




int main(){
    fg_procs = newNode(0);
    char *line = malloc(MAX_LINE_LEN * sizeof(char)); 
    // strcpy(line,"ls -l|grep 'hello     world'|wc -l -c");
    size_t max_line_len = MAX_LINE_LEN;

    while(1){
        printf("Enter the command: ");
        // fflush(stdout);
        getline(&line, &max_line_len, stdin);
        line[strlen(line)-1] = '\0';
        if(strcmp(line,"exit")==0)
            break;
        process *job;
        int n_proc;
        int background = 0;

        job = parse(line,&n_proc,&background);
        int i, j;
        printf("Number of processes = %d\n",n_proc);
        for (i = 0; i<n_proc; i++) {
            for (j = 0; j<job[i].n_args; j++)
                printf("%s ", job[i].args[j]);
            printf("\nNumber of arguments = %d\n\n",job[i].n_args);
        }
        // char * ls_args[] = { "ls" , "-l", NULL};
        //                    ^ 
        //  use the name ls
        //  rather than the
        //  path to /bin/ls
        // if(fork()==0){
        //     printf("Child process executing %s\n",job[0].args[0]);
        //     // job[0].args[job[0].n_args] = NULL;
        // // execvp(ls_args[0],ls_args);
        // execvp(job[0].args[0],job[0].args);
        // exit(0);
        // }
        // else continue;
        exec_job(job,n_proc,background);
    }
}