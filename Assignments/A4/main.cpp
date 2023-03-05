#include <bits/stdc++.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "defs.h"
using namespace std;

vector<vector<int>> graph(37700);
map<int, Node> users;

#define PUSHUPDATE_THREAD_COUNT 25
#define READPOST_THREAD_COUNT 10

extern void * userSimulator(void *);
extern void * pushUpdate(void *);
extern void * readPost(void *);

// Read from an csv file, edges of a graph, of the form (u,v) in each line and store in a vector of vectors and also intialise a map of users with user_id as key and Node object as value and fill it up with the users in the graph
void load_graph(){
    ifstream file("musae_git_edges.csv");
    string line;
    getline(file, line);
    int u,v;
    while(getline(file, line)){
        stringstream ss(line);
        getline(ss, line, ',');
        u = stoi(line);
        getline(ss, line, ',');
        v = stoi(line);
        graph[u].push_back(v);
        graph[v].push_back(u);
        if(users.find(u) == users.end()){
            users[u] = Node(u);
        }
        if(users.find(v) == users.end()){
            users[v] = Node(v);
        }
        users[u].degree++;
        users[v].degree++;
    }
    for(int i=0;i<users.size();i++) users[i].init();
    file.close();
}

int main(){
    srand(time(NULL));  // seed time only once globally

    load_graph();   // load graph into memory

    // Generate userSimulator thread
    pthread_t userSimulator_thread;
    pthread_attr_t userSimulator_attr;
    if(pthread_attr_init(&userSimulator_attr) < 0){
        exit_with_error("userSimulator::pthread_attr_init() failed");
    }
    if(pthread_create(&userSimulator_thread , &userSimulator_attr , userSimulator , NULL) < 0){
        exit_with_error("userSimulator::pthread_create() failed");
    }

    // Generate 25 pushUpdate threads
    pthread_t pushUpdate_thread[PUSHUPDATE_THREAD_COUNT];
    pthread_attr_t pushUpdate_attr[PUSHUPDATE_THREAD_COUNT];
    for(int i=0;i<PUSHUPDATE_THREAD_COUNT;i++){
        if(pthread_attr_init(&pushUpdate_attr[i]) < 0){
            exit_with_error("pushUpdate::pthread_attr_init() failed");
        }
        if(pthread_create(&pushUpdate_thread[i] , &pushUpdate_attr[i] , pushUpdate , (void *)(uintptr_t)i) < 0){
            exit_with_error("pushUpdate::pthread_create() failed");
        }
    }

    // Generate 10 readPost threads
    pthread_t readPost_thread[READPOST_THREAD_COUNT];
    pthread_attr_t readPost_attr[READPOST_THREAD_COUNT];
    for(int i=0;i<READPOST_THREAD_COUNT;i++){
        if(pthread_attr_init(&readPost_attr[i]) < 0){
            exit_with_error("readPost::pthread_attr_init() failed");
        }
        if(pthread_create(&readPost_thread[i] , &readPost_attr[i] , readPost , (void *)(uintptr_t)i) < 0){
            exit_with_error("readPost::pthread_create() failed");
        }
    }

    // Join all threads
    if(pthread_join(userSimulator_thread, NULL)){
        exit_with_error("userSimulator::pthread_join() failed");
    }

    for(int i=0;i<PUSHUPDATE_THREAD_COUNT;i++){
        if(pthread_join(pushUpdate_thread[i] , NULL) < 0){
            exit_with_error("pushUpdate::pthread_join() failed");
        }
    }

    for(int i=0;i<READPOST_THREAD_COUNT;i++){
        if(pthread_join(readPost_thread[i] , NULL) < 0){
            exit_with_error("readPost::pthread_join() failed");
        }
    }

    // Destroy all threads' attributes
    if(pthread_attr_destroy(&userSimulator_attr) < 0){
        exit_with_error("userSimulator::pthread_attr_ddestroy() failed");
    }

    for(int i=0;i<PUSHUPDATE_THREAD_COUNT;i++){
        if(pthread_attr_destroy(&pushUpdate_attr[i])){
            exit_with_error("pushUpdate::pthread_attr_destroy() failed");
        }
    }

    for(int i=0;i<READPOST_THREAD_COUNT;i++){
        if(pthread_attr_destroy(&readPost_attr[i])){
            exit_with_error("readPost::pthread_attr_destroy() failed");
        }
    }

    return 0;
}

/*
for getting priority of all nodes for each node, simply do this:
    for each node
        for all pairs of direct neighbors x,y:
            x.priority(y)++
            y.priority(x)++

    Time complexity: O((summation of (Degree choose 2)) * 2 * log(n))
                   = O((summation of degree^2) * log(n))
    
    Improvement heuristic (already inbuilt in above idea): Nodes with no common neigbour have priority 0, so they are not stored
*/     