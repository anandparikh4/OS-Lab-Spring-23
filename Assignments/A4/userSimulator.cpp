#include <bits/stdc++.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "defs.h"
using namespace std;

#define RANDOM_NODE_COUNT 5     // ## Change to 100

extern vector<vector<int>> graph;
extern map<int, Node> users;

void *userSimulator(void *arg){
    while(1){
        for(int i=0; i<RANDOM_NODE_COUNT; i++){
            int random_node = rand()%users.size();
            cout << random_node << " " << users[random_node].degree << " " << users[random_node].log_degree << endl;
            int num_actions = ceil(users[random_node].log_degree);

            for(int j=0; j<num_actions; j++){
                int action_type = rand()%3;
                long timestamp = time(0);
                Action action(random_node , ++users[random_node].num_action[action_type] , timestamp , action_type);
                // action.print();

                users[random_node].wall.push_back(action);     // Push to Wall queue of user
            }

            
        }

        // sleep(10);      // ## reset to this
        break;
    }
    pthread_exit(NULL);
}