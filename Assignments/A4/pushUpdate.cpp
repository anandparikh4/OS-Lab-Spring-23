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
extern struct global_lock global_lock;

void * pushUpdate(void * param){
    int id = (intptr_t)param;
    int prev_iter = 0;
    
    if(pthread_mutex_lock(&global_lock.shared_lock) < 0){
        exit_with_error("pushUpdate::pthread_mutex_lock() failed");
    }
    
    while(1){
        while(curr_iter <= prev_iter){
            if(pthread_cond_wait(&global_lock.shared_cond , &global_lock.shared_lock) < 0){
                exit_with_error("pushUpdate::pthread_cond_wait() failed");
            }
        }
        prev_iter = curr_iter;
        cout << "pushUpdate[" << id << "] has lock: " << curr_iter << endl;
        // read from shared queue
        if(pthread_cond_broadcast(&global_lock.shared_cond) < 0){
            exit_with_error("pushUpdate::pthread_cond_broadcast() failed");
        }
        if(pthread_cond_wait(&global_lock.shared_cond , &global_lock.shared_lock) < 0){
            exit_with_error("pushUpdate::pthread_cond_wait() failed");
        }
    }

    pthread_exit(0);
}