#include <bits/stdc++.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "defs.h"
using namespace std;

vector<vector<int>> graph(38000);
map<int, Node> users;

extern void * userSimulator(void *);

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
    pthread_attr_t userSimulator_attr;
    if(pthread_attr_init(&userSimulator_attr) < 0){
        exit_with_error("userSimulator::pthread_attr_init() failed");
    }
    if(pthread_create(&userSimulator_thread , &userSimulator_attr , userSimulator , NULL) < 0){
        exit_with_error("userSimulator::pthread_create() failed");
    }
    if(pthread_join(userSimulator_thread, NULL)){
        exit_with_error("userSimulator::pthread_join() failed");
    }

    // for(auto it=users.begin(); it!=users.end(); it++){     // Check if the wall of a user has been updated
    //     if(it->second.wall.size() > 0) {
    //         cout << "Wall of user " << it->first << " has been updated" << endl;
    //     }
    // }

    return 0;
}
            