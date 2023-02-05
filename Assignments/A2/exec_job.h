#ifndef _EXEC_JOB_H
#define _EXEC_JOB_H
#define MAX_ARGS 256

#include <stdlib.h>
#include <unistd.h>
#include "process.h"
#include <sys/wait.h>
#include <set>



void exec_job(process * job , int n_proc , int background);

void exec_proc(process * p, int infd, int outfd);

void redirect(process * proc , int infd , int outfd);
#endif