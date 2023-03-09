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

vector<vector<Action>> shared(RANDOM_NODE_COUNT);
my_semaphore write_shared(1),read_shared(0);
int curr_iter = 0;

bool cmp(const pair<int,vector<Action>> &a , const pair<int,vector<Action>> &b){
    return a.first > b.first;
}

void *userSimulator(void *arg){

    while(1){
        vector<pair<int,vector<Action>>> temp_shared(RANDOM_NODE_COUNT);

        for(int i=0; i<RANDOM_NODE_COUNT; i++){
            int random_node = rand()%users.size();
            // cout << random_node << " " << users[random_node].degree << " " << users[random_node].log_degree << endl;
            int num_actions = ACTIONS_PROPORTIONALITY_CONSTANT*floor(1+users[random_node].log_degree);
            
            for(int j=0; j<num_actions; j++){
                int action_type = rand()%3;
                long timestamp = time(0);
                Action action(random_node , ++users[random_node].num_action[action_type] , timestamp , action_type);
                users[random_node].wall.push_back(action);     // Push to Wall queue of user
                temp_shared[i].second.push_back(action);
            }
            temp_shared[i].first = temp_shared[i].second.size() * graph[random_node].size();
        }

        // sort(temp_shared.begin() , temp_shared.end() , cmp);        // sort by decreasing number of total number of pushes

        write_shared._wait();
        // write to shared
        curr_iter++;
        //Minimum number of actions in a node's wall
        cout<<"Minimum number of actions in a node's wall : "<<temp_shared[RANDOM_NODE_COUNT-1].second.size()<<endl;
        for(int i=0;i<RANDOM_NODE_COUNT;i++){
            shared[(i+curr_iter)%RANDOM_NODE_COUNT] = temp_shared[i].second;    // round-robin load balancing        
        }
        read_shared._signal();

        write_logfile._wait();
        // write to log file
        logfile << "---------------------------------------------------------------------------\n";
        logfile << "userSimulator iteration #" << curr_iter << " : " << endl;
        for(int i=0;i<RANDOM_NODE_COUNT;i++){
            logfile<<"Node "<<i<<" : "<<endl;
            for(int j=0;j<temp_shared[i].second.size();j++) logfile << temp_shared[i].second[j] << endl;
        }
        write_logfile._signal();

        sleep(SLEEP_SECONDS);
    }

    pthread_exit(NULL);
}