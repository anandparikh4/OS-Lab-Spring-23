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
extern ofstream logfile;
extern my_semaphore write_logfile;
extern my_semaphore write_inform,read_inform;
extern int test_count;
extern vector<int> inform;

my_semaphore rP_group(1);
int start = 0;
int finish = 0;
bool done_rp[READPOST_THREAD_COUNT] = {false};
string extn_rp = ".rP_txt";

// cmp function for sorting the feed of a user on the basis of priority of the poster node
bool cmp(const Action &a, const Action &b, const int c){
    return users[c].priority[a.user_id] > users[c].priority[b.user_id];
}


void * readPost(void * param){
    int id = (intptr_t)param;
    int prev_iter = 0;
    // string file_name;
    // file_name.push_back('0' + id);
    // file_name += extn_rp;
    // ofstream testfile;
    // testfile.open(file_name , std::ios_base::app);
    // int temp_count = 0;

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

        prev_iter = test_count / PUSHUPDATE_THREAD_COUNT;
        // All readPost threads have CONCURRENT access to this code
        // read from "inform" queue
        // temp_count = test_count;
        vector<int> temp_inform;
        // Read concurrently from "inform" queue
        for(int i=0;i<ceil(((double)inform.size())/READPOST_THREAD_COUNT);i++){
            if((id+i*READPOST_THREAD_COUNT) >= inform.size()) break;
            temp_inform.push_back(inform[id+i*READPOST_THREAD_COUNT]);
        }
        rP_group._wait();
        finish++;
        done_rp[id] = true;
        if(finish == READPOST_THREAD_COUNT){
            start = 0;
            finish = 0;
            for(int i=0;i<READPOST_THREAD_COUNT;i++) done_rp[i] = false;
            // cout<<"All readPost threads have finished their reading for iteration #"<<prev_iter<<endl;
            write_inform._signal();
        }
        rP_group._signal();
        stringstream ss;
        ss << "readPost[" << id << "] iteration #: " << prev_iter << "\n";
        // testfile << "Size of temp_inform = " << temp_inform.size() << endl;
        for(auto i:temp_inform){
            users[i].feedsem._wait();
            if(users[i].feed.size()){
                if(users[i].sort_by==0){
                    sort(users[i].feed.begin(),users[i].feed.end(),[&](const Action &a, const Action &b){
                        return users[i].priority[a.user_id] > users[i].priority[b.user_id];
                    });
                }
                // testfile << "Sort Status = " << ((users[i].sort_by==0)?"Yes":"No") << endl;
                // testfile << "Zero element priority = " << users[i].priority[users[i].feed[0].user_id] << endl;
                // testfile << "Last element priority = " << users[i].priority[users[i].feed[users[i].feed.size()-1].user_id] << endl;
                ss << "User " << i << " has " << users[i].feed.size() << " posts in his feed."<<"\n";
                ss << "Reading feed of user in " << ((users[i].sort_by==0)?"Priority based":"Chronological") << " order."<<"\n";
                for(auto x:users[i].feed){
                    ss << "I read action number " << x.action_id << " of type "<< x.action_type << " posted by user " << x.user_id << " at time " << x.timestamp;
                    if(users[i].sort_by==0) ss << " with priority " << users[i].priority[x.user_id]<<"\n";
                    else    ss<<"\n";
                }
                ss<<"--------------------------------------------"<<"\n";
                users[i].feed.clear();
            } 
            users[i].feedsem._signal();
        }
        ss << "---------------------------------------------------------------------------"<<endl;
        // cout<<"readPost["<<id<<"] has finished its work for iteration #"<<prev_iter<<endl;

        write_logfile._wait();
        // write to logfile
        logfile << ss.str();
        cout << "Completed: readPost[" << id << "]" << ": iteration #" << prev_iter << endl;
        write_logfile._signal();

        // write_stdout._wait();
        // write_stdout._signal();
    }
    
    pthread_exit(0);
}