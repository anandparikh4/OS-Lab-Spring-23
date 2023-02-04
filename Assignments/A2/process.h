#ifndef _PROCESS_H
#define _PROCESS_H
#define MAX_ARGS 256

struct process{
    char * args[MAX_ARGS];
    int n_args;
};

void init_proc(struct process * p){     // initialize process structure content
    for(int i=0;i<MAX_ARGS;i++){
        p->args[i] = NULL;
    }
    p->n_args = 0;
    return;
}

#endif