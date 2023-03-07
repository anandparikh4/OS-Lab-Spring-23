#include <bits/stdc++.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "defs.h"
using namespace std;

extern int curr_iter;
extern vector<vector<int>> graph;
extern map<int, Node> users;
extern my_semaphore shared_sem,logfile_sem,rP_group_sem;

extern my_semaphore write_to_shared,read_from_shared;
my_semaphore pU_group_sem;
int rc = 0;
int done = 0;

void * pushUpdate(void * param){
    int id = (intptr_t)param;

    while(1){
        pU_group_sem._wait();
        if(rc == 0 && done == 0){
            read_from_shared._wait();
        }
        rc++;
        pU_group_sem._signal();

        // read
        for(int i=0;i<100;i++) printf("%c" , 'a' + id);

        pU_group_sem._wait();
        rc--;
        done++;
        if(rc == 0 && done == 25){
            done = 0;
            write_to_shared._signal();
        }
        pU_group_sem._signal();
    }

    pthread_exit(0);
}