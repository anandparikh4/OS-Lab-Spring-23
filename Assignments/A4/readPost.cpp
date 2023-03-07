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
extern ofstream logfile;
extern my_semaphore write_logfile;
extern my_semaphore write_inform,read_inform;
extern int test_count;

my_semaphore rP_group(1);
int start = 0;
int finish = 0;
bool done_rp[READPOST_THREAD_COUNT] = {false};
string extn_rp = ".rP_txt";

void * readPost(void * param){
    int id = (intptr_t)param;
    int prev_iter = 0;
    string file_name;
    file_name.push_back('1' + id);
    file_name += extn_rp;
    ofstream testfile;
    testfile.open(file_name , std::ios_base::app);
    int temp_count = 0;

    while(1){
        
        while(1){
            rP_group._wait();
            if(done_rp[id]){
                rP_group._signal();
                continue;
            }
            break;
        }
        if(start == 0){
            read_inform._wait();
        }
        start++;
        rP_group._signal();

        prev_iter = curr_iter;
        // All readPost threads have CONCURRENT access to this code
        // read from "inform" queue
        temp_count = test_count;

        rP_group._wait();
        finish++;
        done_rp[id] = true;
        if(finish == READPOST_THREAD_COUNT){
            start = 0;
            finish = 0;
            for(int i=0;i<READPOST_THREAD_COUNT;i++) done_rp[i] = false;
            // cout<<"All readPost threads have finished their work for iteration #"<<prev_iter<<endl;
            write_inform._signal();
        }
        rP_group._signal();

        testfile << "---------------------------------------------------------------------------\n";
        testfile << "readPost[" << id << "] iteration #: " << prev_iter << "| count = " << temp_count << endl;

        // write_logfile._wait();
        // // write to logfile
        // write_logfile._signal();
    }
    
    pthread_exit(0);
}