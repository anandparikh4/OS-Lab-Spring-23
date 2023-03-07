#include <bits/stdc++.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "defs.h"
using namespace std;

extern vector<vector<int>> graph;
extern map<int, Node> users;
extern my_semaphore write_logfile;
extern ofstream logfile;

void * readPost(void * param){
    int id = (intptr_t)param;

    // while(1){
    //     write_logfile._wait();
    //     // write to logfile
    //     write_logfile._signal();
    // }
    
    pthread_exit(0);
}