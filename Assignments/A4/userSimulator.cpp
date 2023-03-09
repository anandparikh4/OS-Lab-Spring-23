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
extern my_semaphore write_logfile;
extern ofstream logfile;
extern my_semaphore write_stdout;

vector<vector<Action>> shared(BATCH_SIZE);
my_semaphore write_shared(1),read_shared(0);
int curr_uS_iter = 0;

bool cmp(const pair<int,vector<Action>> &a , const pair<int,vector<Action>> &b){
    return a.first > b.first;
}

void *userSimulator(void *arg){

    while(1){
        for(int batch = 0;batch<NUM_BATCHES;batch++){        
            vector<vector<Action>> temp_shared(BATCH_SIZE);

            for(int i=0; i<BATCH_SIZE; i++){
                int random_node = rand()%users.size();
                // cout << random_node << " " << users[random_node].degree << " " << users[random_node].log_degree << endl;
                int num_actions = ACTIONS_PROPORTIONALITY_CONSTANT*floor(1+users[random_node].log_degree);
                
                for(int j=0; j<num_actions; j++){
                    int action_type = rand()%3;
                    long timestamp = time(0);
                    Action action(random_node , ++users[random_node].num_action[action_type] , timestamp , action_type);
                    users[random_node].wall.push_back(action);     // Push to Wall queue of user
                    temp_shared[i].push_back(action);
                }
                // temp_shared[i].first = temp_shared[i].second.size() * graph[random_node].size();
            }

            // sort(temp_shared.begin() , temp_shared.end() , cmp);        // sort by decreasing number of total number of pushes

            write_shared._wait();
            // write to shared
            curr_uS_iter++;
            //Minimum number of actions in a node's wall
            // cout<<"Minimum number of actions in a node's wall : "<<temp_shared[BATCH_SIZE-1].second.size()<<endl;
            for(int i=0;i<BATCH_SIZE;i++){
                shared[(i+curr_uS_iter)%BATCH_SIZE] = temp_shared[i];    // round-robin load balancing        
            }
            read_shared._signal();

            write_logfile._wait();
            // write to log file
            logfile << "---------------------------------------------------------------------------\n";
            logfile << "userSimulator iteration #" << curr_uS_iter << " : " << endl;
            for(int i=0;i<BATCH_SIZE;i++){
                logfile<<"Node "<<i<<" : "<<endl;
                for(int j=0;j<temp_shared[i].size();j++) logfile << temp_shared[i][j] << endl;
            }
            write_logfile._signal();

            write_stdout._wait();
            cout << "userSimulator: iteration #" << curr_uS_iter<< endl;
            write_stdout._signal();
        }

        sleep(SLEEP_SECONDS);
    }

    pthread_exit(NULL);
}