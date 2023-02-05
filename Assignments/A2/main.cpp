#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "parse.h" 
#include "exec_job.h"
#include <set>
#include <sys/signal.h>
#include <iostream>

using namespace std;
#define MAX_LINE_LEN 1024

int foreground_pgid;
set <int> fg_procs,bg_run_procs,bg_stop_procs;

void sigint_handler(int signum){
    signal(SIGINT,sigint_handler);
    // printf("Inside signal handler for Ctrl-C\n");
    for(auto &it:fg_procs){
        kill(it,SIGINT);
    }
    printf("\nEnter the command: ");
    return;
}

void sigchild_handler(int signum){
    signal(SIGCHLD,sigchild_handler);
    while(1){
        int status;
        int cpid = waitpid(-1,&status,WUNTRACED|WNOHANG|WCONTINUED); //WCONTINUED check
        if(cpid<=0)break;
        if(fg_procs.find(cpid)!=fg_procs.end()){
            fg_procs.erase(cpid);
            if(WIFSTOPPED(status))bg_stop_procs.insert(cpid);
        }
        else if(bg_run_procs.find(cpid)!=bg_run_procs.end()){
            bg_run_procs.erase(cpid);
            if(WIFSTOPPED(status))bg_stop_procs.insert(cpid);
        }
    
    }
    
}

void sigchld_blocker(int signal_state){
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set , SIGCHLD);
    sigprocmask(signal_state , &signal_set , NULL);
}

int main(){
    fg_procs.clear();
    bg_run_procs.clear();
    bg_stop_procs.clear();
    //signal(SIGINT,sigint_handler);
    signal(SIGCHLD,sigchild_handler);
    signal(SIGTTOU,SIG_IGN);
    foreground_pgid = 0;

    char *line = (char*)malloc(MAX_LINE_LEN * sizeof(char)); 
    // strcpy(line,"ls -l|grep 'hello     world'|wc -l -c");
    size_t max_line_len = MAX_LINE_LEN;

    while(1){
        //fflush(stdout);
        printf("Enter the command: ");
        getline(&line, &max_line_len, stdin);
        line[strlen(line)-1] = '\0';
        if(strcmp(line,"exit")==0)break;
        process *job;
        int n_proc;
        int background = 0;

        job = parse(line,&n_proc,&background);
        int i, j;
        // printf("Number of processes = %d\n",n_proc);
        // for (i = 0; i<n_proc; i++) {
        //     for (j = 0; j<job[i].n_args; j++)
        //         printf("%s ", job[i].args[j]);
        //     printf("\nNumber of arguments = %d\n\n",job[i].n_args);
        // }
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