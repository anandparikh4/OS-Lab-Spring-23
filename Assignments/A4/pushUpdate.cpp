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

extern my_semaphore write_shared,read_shared;
my_semaphore pU_group(1);
int start = 0;
int finish = 0;
bool done[25] = {false};

void * pushUpdate(void * param){
    int id = (intptr_t)param;

    while(1){
        pU_group._wait();
        if(done[id]){
            pU_group._signal();
            continue;
        }
        if(start == 0){
            read_shared._wait();
        }
        start++;
        pU_group._signal();

        // read
        for(int i=0;i<1000;i++) printf("%c" , 'a' + id);

        pU_group._wait();
        finish++;
        done[id] = true;
        if(finish == PUSHUPDATE_THREAD_COUNT){
            start = 0;
            finish = 0;
            for(int i=0;i<PUSHUPDATE_THREAD_COUNT;i++) done[id] = 0;
            write_shared._signal();
        }
        pU_group._signal();
    }

    pthread_exit(0);
}