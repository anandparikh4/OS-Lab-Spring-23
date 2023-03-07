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
extern vector<int> shared_vec;

extern my_semaphore write_shared,read_shared;
my_semaphore pU_group(1);
int start = 0;
int finish = 0;
bool done[PUSHUPDATE_THREAD_COUNT] = {false};
string extn = ".txt";

void * pushUpdate(void * param){
    int id = (intptr_t)param;
    string file_name;
    file_name.push_back('a'+id);
    file_name += extn;

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
        ofstream File;
        File.open(file_name);
        File << shared_vec[id] << endl;
        File.close();

        pU_group._wait();
        finish++;
        done[id] = true;
        if(finish == PUSHUPDATE_THREAD_COUNT){
            start = 0;
            finish = 0;
            for(int i=0;i<PUSHUPDATE_THREAD_COUNT;i++) done[i] = false;
            write_shared._signal();
        }
        pU_group._signal();
    }

    pthread_exit(0);
}