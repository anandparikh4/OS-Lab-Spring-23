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

void *userSimulator(void *arg) {
    srand(time(0));

    while(1){
        vector<int> random_nodes;
        for(int i=0; i<5; i++) {
            int random_node = rand()%users.size();
            random_nodes.push_back(random_node);
        }

        for(int i=0; i<random_nodes.size(); i++) {
            int n = ceil(users[random_nodes[i]].log_degree);
            for(int j=0; j<n; j++) {
                int action_type = rand()%3;
                long timestamp = time(0);
                Action action(random_nodes[i], ++users[random_nodes[i]].num_actions, timestamp, action_type);
                action.print();
                users[random_nodes[i]].wall.push_back(action);
            }
        }
        sleep(120);
    }
    pthread_exit(NULL);
}