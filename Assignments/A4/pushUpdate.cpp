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
extern vector<vector<Action>> shared;
extern my_semaphore write_shared,read_shared;
extern my_semaphore write_logfile;
extern ofstream logfile;

extern int curr_iter;
my_semaphore pU_group(1);
int start = 0;
int finish = 0;
bool done[PUSHUPDATE_THREAD_COUNT] = {false};
string extn = ".pU_txt";

void * pushUpdate(void * param){
    int id = (intptr_t)param;
    int prev_iter = 0;
    string file_name;
    file_name.push_back('a' + id);
    file_name += extn;
    vector<vector<Action>> temp_shared(RANDOM_NODE_COUNT / PUSHUPDATE_THREAD_COUNT);
    ofstream testfile;
    testfile.open(file_name , std::ios_base::app);

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

        prev_iter = curr_iter;
        for(int i=0;i<RANDOM_NODE_COUNT / PUSHUPDATE_THREAD_COUNT;i++) temp_shared[i] = shared[id+i*PUSHUPDATE_THREAD_COUNT];     // ## Change 2 to 4

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

        // writing to respective test files
        testfile << "---------------------------------------------------------------------------\n";
        testfile << "pushUpdate[" << id << "] iteration #: " << prev_iter << endl;
        for(int i=0;i<RANDOM_NODE_COUNT / PUSHUPDATE_THREAD_COUNT;i++){
            for(int j=0;j<temp_shared[i].size();j++) testfile << temp_shared[i][j] << endl;
        }

        // write_logfile._wait();
        // // write to logfile
        // write_logfile._signal();
    }

    testfile.close();
    pthread_exit(0);
}