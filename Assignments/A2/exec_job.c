#include <stdlib.h>
#include "process.h"
#include <sys/wait.h>
#include "avl_tree.h"

extern struct Node * fg_procs;
extern void exec_proc(struct process *, int , int);

// execute the given job (list of processes)
void exec_job(struct process * job , int n_proc , int background){
    
    // setup pipes between processes
    int connect_fd = 0;             // output of previous pipe (STDIN for first process)
    for(int i=0;i<n_proc;i++){
        int infd = connect_fd;
        int outfd = 1;
        if(i < n_proc-1){           // no pipes case automatically handled
            int fd[2];
            if(pipe(fd) < 0){
                perror("pipe");
                exit(0);
            }
            outfd = fd[0];
            connect_fd = fd[1];
        }
        exec_proc(&job[i] , infd , outfd);
    }

    while(!background && n_proc){        // while some procs are running in the foreground, wait
        int proc_status;
        int child_pid;
        if((child_pid = waitpid(-1 , &proc_status , 0)) < 0){    // wait on all children, blocking, no flag set
            perror("waitpid");
            exit(0);
        }
        if(findNode(fg_procs , child_pid) != NULL){
            n_proc--;                           // child that exited was a foreground process, so decrement n_procs
            deleteNode(fg_procs , child_pid);   // and remove from set
        }
    }

    return;
}