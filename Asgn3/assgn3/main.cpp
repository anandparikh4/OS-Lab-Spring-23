#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <sys/signal.h>
#include "graph.h"

using namespace std;

// The code is written overlooking cases when the producer and some consumer(s) are accessing the shared memory simultaneously
// Race conditions arising in producer and consumer can potentially lead to segment faults/ other errors
// But such issues have been ignored as per the assignment problem statement
// File locks can solve this problem

vector <int> cpids;
graph *G;
// The only normal way to terminate this process is if it is sent a SIGINT, (other abnormal ways like error in children can also cause it)
// So, this process needs to handle SIGINT, where first it terminates (forcefully) the producer and all consumer processes
// Then, the shared memory must be destroyed (marked for de-allocation) otherwise stale/unused shared memory stays forever, leading to a memory leak
void handle_sigint(int num){
    for(auto it:cpids){
        kill(it,SIGINT);
    }
    deactivate_graph(G,2);
    exit(0);
}


int main(int argc, char * argv[]){
    signal(SIGINT,handle_sigint);
    set_swap(0);
    G = activate_graph(1);       // create and attach graph
    int do_optimize = 0;
    if(argc > 1 &&  argc < 3){
        if(strcasecmp(argv[1],"-optimize") == 0){
            do_optimize = 1;
        }
    }
    else if(argc > 2){
        cout<<"Too many arguments\n";
        cout<<"Usage: ./main [-optimize]\n";
        exit(0);
    }


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
    cpids.push_back(producer_pid);
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
            if(do_optimize == 1){
                // cout<<"Optimized version\n";
                char * args[] = {strdup("./consumer") , consumer_id ,file_name,strdup("-optimize"),NULL};
                execvp(args[0] , args);
            }
            else{
                // cout<<"Un-Optimized version\n";
                char * args[] = {strdup("./consumer") , consumer_id ,file_name,NULL};
                execvp(args[0] , args);
            }
            perror("execvp");
            exit(0);
        }
        cpids.push_back(consumer_pid[i]);
    }

    for(int i=0;i<11;i++) wait(NULL);       // wait for each of the 11 children to finish, so PCBs can be cleared and shared memory can be freed safely

    deactivate_graph(G,2);       // detach and destroy graph AND the swap key
    return 0;
}