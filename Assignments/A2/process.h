#ifndef _PROCESS_H
#define _PROCESS_H
#define MAX_ARGS 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct{
    char * args[MAX_ARGS];
    int n_args;
}process;

// void init_proc(process * p);

#endif