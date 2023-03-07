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
extern my_semaphore shared_sem,logfile_sem,pU_group_sem,rP_group_sem;

void * readPost(void * param){
    int id = (intptr_t)param;
    // cout << id << endl;
    pthread_exit(0);
}