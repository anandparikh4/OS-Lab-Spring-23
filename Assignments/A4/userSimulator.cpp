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

// Create a userSimulator thread function, which will choose 100 random nodes from the graph. Then for each node, generate n actions, n needs to be proportional to the log_degree of the node.
void *userSimulator(void *arg) {
    // Seed the random number generator
    srand(time(0));
    while(1){
        // Choose 100 random nodes from the graph
        vector<int> random_nodes;
        for(int i=0; i<5; i++) {
            int random_node = rand()%users.size();
            random_nodes.push_back(random_node);
        }
        // For each node, generate n actions, n needs to be proportional to the log_degree of the node
        for(int i=0; i<random_nodes.size(); i++) {
            int n = ceil(users[random_nodes[i]].log_degree);
            for(int j=0; j<n; j++) {
                cout<<"hwebf"<<endl;
                // Generate a random action type
                int action_type = rand()%3;
                // Generate a current timestamp
                long timestamp = time(0);
                // Create an Action object
                Action action(random_nodes[i], ++users[random_nodes[i]].num_actions, timestamp, action_type);
                action.print();
                // Add the action to the wall and feed of the user
                users[random_nodes[i]].wall.push_back(action);
            }
        }
        // sleep(120);
        break;
    }
    pthread_exit(NULL);
}