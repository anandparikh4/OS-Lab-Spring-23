#ifndef _PROCESS_H
#define _PROCESS_H
#define MAX_ARGS 256

struct process{
    char * args[MAX_ARGS] = {nullptr};
    int n_args = 0;
};

#endif