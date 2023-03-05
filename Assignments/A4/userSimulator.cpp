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
extern queue<pair<int,vector<Action>>> shared;
extern queue<vector<int>> shared_node_freq;
extern int actions_remaining;


void *userSimulator(void *arg){

    while(1){
        
        // while(actions_remaining == RANDOM_NODE_COUNT || shared.size() > RANDOM_NODE_COUNT){
        //     if(pthread_mutex_lock(&global_lock.shared_lock) < 0){
        //         exit_with_error("userSimulator::pthread_mutex_lock() failed");
        //     }
        //     while(actions_remaining && shared.front().first == PUSHUPDATE_THREAD_COUNT){
        //         shared.pop();
        //         actions_remaining--;
        //     }
        //     if(pthread_cond_broadcast(&global_lock.shared_cond) < 0){
        //         exit_with_error("userSimulator::pthread_cond_signal() failed");
        //     }
        //     if(pthread_mutex_unlock(&global_lock.shared_lock) < 0){
        //         exit_with_error("userSimulator::pthread_mutex_unlock() failed");
        //     }
        // }
        // shared_node_freq.clear();
        // vector<vector<Action>> curr_batch(RANDOM_NODE_COUNT);
        for(int i=0; i<RANDOM_NODE_COUNT; i++){
            int random_node = rand()%users.size();
            // cout << random_node << " " << users[random_node].degree << " " << users[random_node].log_degree << endl;
            int num_actions = ceil(users[random_node].log_degree);
            
            for(int j=0; j<num_actions; j++){
                int action_type = rand()%3;
                long timestamp = time(0);
                Action action(random_node , ++users[random_node].num_action[action_type] , timestamp , action_type);
                users[random_node].wall.push_back(action);     // Push to Wall queue of user
                // curr_batch[i].push_back(action);
            }
        }


        if(pthread_mutex_lock(&global_lock.shared_lock) < 0){
            exit_with_error("userSimulator::pthread_mutex_lock() failed");
        }
        curr_iter++;
        cout << "userSimulator has lock: " << curr_iter << endl;
        // push all generated actions to shared queue

        // remove used elements
        // if(actions_remaining>0){
        //     while(actions_remaining && shared.front().first == PUSHUPDATE_THREAD_COUNT){
        //         shared.pop();
        //         actions_remaining--;
        //     }
        // }
        // if(actions_remaining==0){
        //     actions_remaining = RANDOM_NODE_COUNT;
        //     shared_node_freq.pop();
        // }    
        // // add new elements
        // for(int i=0; i<RANDOM_NODE_COUNT; i++){
        //     shared.push({0, curr_batch[i]});
        // }

        if(pthread_cond_broadcast(&global_lock.shared_cond) < 0){
            exit_with_error("userSimulator::pthread_cond_signal() failed");
        }
        if(pthread_mutex_unlock(&global_lock.shared_lock) < 0){
            exit_with_error("userSimulator::pthread_mutex_unlock() failed");
        }

        // if(pthread_mutex_lock(&global_lock.logfile_lock) < 0)

        sleep(SLEEP_SECONDS);
    }
    pthread_exit(NULL);
}