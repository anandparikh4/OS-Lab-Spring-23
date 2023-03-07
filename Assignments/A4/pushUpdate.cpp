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
extern int curr_iter;
extern vector<vector<Action>> shared;
extern ofstream logfile;
extern my_semaphore write_logfile;
extern my_semaphore write_shared,read_shared;

my_semaphore write_inform(1),read_inform(0);
my_semaphore pU_group(1);
vector<vector<Action>> inform;
int start_read = 0;
int finish_read = 0;
int written = 0;
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

        while(1){
            pU_group._wait();
            if(done[id]){
                pU_group._signal();
                continue;
            }
            break;
        }
        if(start_read == 0){
            read_shared._wait();
        }
        start_read++;
        pU_group._signal();

        // All pushUpdate threads have CONCURRENT access to this code
        // read from "shared" queue
        prev_iter = curr_iter;
        for(int i=0;i<RANDOM_NODE_COUNT / PUSHUPDATE_THREAD_COUNT;i++) temp_shared[i] = shared[id+i*PUSHUPDATE_THREAD_COUNT];     // ## Change 2 to 4

        pU_group._wait();
        finish_read++;
        if(finish_read == PUSHUPDATE_THREAD_COUNT){
            start_read = 0;
            finish_read = 0;
            write_shared._signal();
        }
        pU_group._signal();

        // writing to respective test files
        testfile << "---------------------------------------------------------------------------\n";
        testfile << "pushUpdate[" << id << "] iteration #: " << prev_iter << endl;
        for(int i=0;i<RANDOM_NODE_COUNT / PUSHUPDATE_THREAD_COUNT;i++){
            for(int j=0;j<temp_shared[i].size();j++) testfile << temp_shared[i][j] << endl;
        }

        pU_group._wait();
        if(written == 0){
            write_inform._wait();
        }

        // All pushUpdate threads have SEQUENTIAL access to this code
        // write to "inform" queue

        written++;
        done[id] = true;
        if(written == PUSHUPDATE_THREAD_COUNT){
            written = 0;
            for(int i=0;i<PUSHUPDATE_THREAD_COUNT;i++) done[i] = false;
            read_inform._wait();
        }
        pU_group._signal();

        // write_logfile._wait();
        // // write to logfile
        // write_logfile._signal();
    }

    testfile.close();
    pthread_exit(0);
}