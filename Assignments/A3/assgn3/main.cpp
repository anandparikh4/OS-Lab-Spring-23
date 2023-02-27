#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <fstream>
#include <vector>
#include <cstring>
#include "graph.h"

using namespace std;

// The code is written overlooking cases when the producer and some consumer(s) are accessing the shared memory simultaneously
// It is possible that when the producer and a consumer are both working on the memory, the producer reallocates a new space (space full)
// Then the old space is de-allocated but the consumer still tries to access it, leading to a disastrous segmentation fault
// Conditions similar to (including) the above one are IGNORED as per the assignment problem statement
// File locks can solve this problem

int main(){

    set_swap(0);
    graph * G;
    G = activate_graph(1);       // create and attach graph

    ifstream File;
    File.open("graph.txt");     // read file for edge list and store temporarily in a local storage
    vector<int> left;
    vector<int> right;
    int n = -1,temp,side = 0;
    while(File >> temp){
        if(side == 0) left.push_back(temp);
        else right.push_back(temp);
        n = max(n , temp);
        side = 1 - side;
    }
    File.close();

    for(int i=0;i<=n;i++){      // first add all nodes
        G = add_node(G , i);
    }

    for(int i=0;i<min(left.size(),right.size());i++){       // then add all edges
        G = add_edge(G , left[i] , right[i]);
    }

    int producer_pid = fork();
    if(producer_pid < 0){
        perror("fork");
        exit(0);
    }

    if(producer_pid == 0){                  // execute producer child process
        char * args[] = {strdup("./producer") , NULL};
        execvp(args[0] , args);
        perror("execvp");
        exit(0);
    }

    int consumer_pid[10];
    for(int i=0;i<10;i++){                  // execute 10 consumer child processes
        consumer_pid[i] = fork();
        if(consumer_pid[i] < 0){
            perror("fork");
            exit(0);
        }
        
        if(consumer_pid[i] == 0){
            char consumer_id[10];
            sprintf(consumer_id , "%d" , i+1);
            char file_name[20];
            sprintf(file_name, "paths_%d.txt",i+1);
            //uncomment below line for non-optimized version
            //char * args[] = {strdup("./consumer") , consumer_id ,file_name,strdup("-optimize"),NULL};
            
            //comment below line for non-optimized version
            char * args[] = {strdup("./consumer") , consumer_id ,file_name,strdup("-optimize"),NULL};
            execvp(args[0] , args);
            perror("execvp");
            exit(0);
        }
    }

    for(int i=0;i<10;i++) wait(NULL);       // wait for each of the 10 children to finish, so PCBs can be cleared and shared memory can be freed safely

    deactivate_graph(G,2);       // detach and destroy graph AND the swap key
    return 0;
}