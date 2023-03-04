#include <bits/stdc++.h>
#include <pthread.h>
// #include <semaphore.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "main.h"
using namespace std;

// Vector of vectors to store the graph
vector<vector<int>> graph(38000);
// Map of users with user_id as key and Node object as value
map<int, Node> users;

// Read from an csv file, edges of a graphy, of the form (u,v) in each line and store in a vector of vectors and also intialise a map of users with user_id as key and Node object as value and fill it up with the users in the graph
void read_graph() {
    // Open the file
    ifstream file("musae_git_edges.csv");
    // Read the file line by line
    string line;
    getline(file, line);
    int u,v;
    // Take input in the form u,v from each line and store in graph
    while(getline(file, line)) {
        stringstream ss(line);
        getline(ss, line, ',');
        u = stoi(line);
        getline(ss, line, ',');
        v = stoi(line);
        graph[u].push_back(v);
        graph[v].push_back(u);
        // If Node u is not present in the map, add it
        if(users.find(u) == users.end()) {
            users[u] = Node(u);
        }
        // If Node v is not present in the map, add it
        if(users.find(v) == users.end()) {
            users[v] = Node(v);
        }
        // Increment the degree of both u and v
        users[u].degree++;
        users[v].degree++;
    }
    // Close the file
    file.close();
}

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

int main(){
    // Read the graph from the csv file
    read_graph();
    // Print the number of users in the graph
    cout << "Number of users in the graph: " << users.size() << endl;
    // Print the number of edges in the graph
    int num_edges = 0;
    for(int i=0; i<graph.size(); i++) {
        num_edges += graph[i].size();
    }
    cout << "Number of edges in the graph: " << num_edges/2 << endl;
    // Find maximum degree of a user
    // int max_degree = 0;
    // for(auto it=users.begin(); it!=users.end(); it++) {
    //     max_degree = max(max_degree, it->second.degree);
    // }
    // cout << "Maximum degree of a user: " << max_degree << endl;
    
    // Generate userSimulator thread
    pthread_t userSimulator_thread;
    if(pthread_create(&userSimulator_thread, NULL, userSimulator,NULL)<0) {
        cout << "Error in creating userSimulator thread" << endl;
        exit(0);
    }    
    pthread_join(userSimulator_thread, NULL);
    // Check if the wall of a user has been updated
    for(auto it=users.begin(); it!=users.end(); it++) {
        // printf("hell\n");
        if(it->second.wall.size() > 0) {
            cout << "Wall of user " << it->first << " has been updated" << endl;
        }
    }
    
    
    return 0;
}
            