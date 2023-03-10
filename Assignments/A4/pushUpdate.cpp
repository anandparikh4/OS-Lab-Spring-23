#include <bits/stdc++.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "defs.h"
using namespace std;

extern vector<vector<int>> graph;
extern vector<Node> users;
extern int curr_uS_iter;
extern vector<vector<Action>> shared;
extern ofstream logfile;
extern my_semaphore write_logfile;
extern my_semaphore write_shared,read_shared;

my_semaphore write_inform(1),read_inform(0);
my_semaphore pU_group(1);
vector<int> inform;
set<int> feed_updates;
int start_read = 0;
int finish_read = 0;
int written = 0;
bool done_pu[PUSHUPDATE_THREAD_COUNT] = {false};
// string extn_pu = ".pU_txt";
int test_count = 0;

void * pushUpdate(void * param){
    int id = (intptr_t)param;
    int prev_iter = 0;
    // string file_name;
    // file_name.push_back('a' + id);
    // file_name += extn_pu;
    vector<vector<Action>> temp_shared(BATCH_SIZE / PUSHUPDATE_THREAD_COUNT);
    // ofstream testfile;
    // testfile.open(file_name , std::ios_base::app);

    while(1){

        while(1){
            pU_group._wait();
            if(done_pu[id]){
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
        prev_iter = curr_uS_iter;
        for(int i=0;i<BATCH_SIZE / PUSHUPDATE_THREAD_COUNT;i++) temp_shared[i] = shared[id+i*PUSHUPDATE_THREAD_COUNT];

        pU_group._wait();
        finish_read++;
        if(finish_read == PUSHUPDATE_THREAD_COUNT){
            start_read = 0;
            finish_read = 0;
            // cout<<"All pushUpdate threads have finished their reading for iteration #"<<prev_iter<<endl;
            write_shared._signal();
        }
        pU_group._signal();

        map<int, int> temp_feed_updates;  // Stores the nodes that will be updated in the current iteration
        for(int i=0;i<BATCH_SIZE / PUSHUPDATE_THREAD_COUNT;i++){
            int user_id = temp_shared[i][0].user_id;
            for(auto x: graph[user_id]){
                temp_feed_updates[x] |= (1<<i);
            }
        }

        for(auto x: temp_feed_updates){
            users[x.first].feedsem._wait();
            for(int i=0;i<BATCH_SIZE / PUSHUPDATE_THREAD_COUNT;i++){
                if(x.second & (1<<i)){
                    for(int j=0;j<temp_shared[i].size();j++){
                        users[x.first].feed.push_back(temp_shared[i][j]);
                    }
                }
            }
            users[x.first].feedsem._signal();
        }

        pU_group._wait();
        if(written == 0){
            write_inform._wait();
        }

        // All pushUpdate threads have SEQUENTIAL access to this code
        // write to "inform" queue
        test_count++;
        // cout << "pushUpdate[" << id << "] iteration #: " << prev_iter << "| count = "  << test_count << endl;
        for(auto x: temp_feed_updates){
            feed_updates.insert(x.first);
        }
        written++;
        done_pu[id] = true;
        if(written == PUSHUPDATE_THREAD_COUNT){
            written = 0;
            for(int i=0;i<PUSHUPDATE_THREAD_COUNT;i++) done_pu[i] = false;
            inform.clear();
            // cout<<"Inform size: "<<feed_updates.size()<<endl;
            for(auto x: feed_updates){
                inform.push_back(x);
                // cout<<"Size of feed of "<<x<<" is "<<users[x].feed.size()<<endl;
            } 
            feed_updates.clear();
            // cout<<"All pushUpdate threads have finished their writing for iteration #"<<prev_iter<<endl;
            read_inform._signal();
        }
        pU_group._signal();

        write_logfile._wait();
        // write to logfile
        logfile << "pushUpdate[" << id << "]" << ": iteration #" << prev_iter << "\n";
        for(int i=0;i<BATCH_SIZE / PUSHUPDATE_THREAD_COUNT;i++){
            logfile << "Read " << temp_shared[i].size() << " actions from shared queue of the user " << temp_shared[i][0].user_id << "\n";
            logfile << "Updated the feed of " << graph[temp_shared[i][0].user_id].size() << " neighbours of the user " << temp_shared[i][0].user_id << "\n";
        }
        logfile << "---------------------------------------------------------------------------"<<endl;
        cout << "Completed: pushUpdate[" << id << "]" << ": iteration #" << prev_iter << endl;
        write_logfile._signal();

        // write_stdout._wait();
        // write_stdout._signal();
    }

    // testfile.close();
    pthread_exit(0);
}