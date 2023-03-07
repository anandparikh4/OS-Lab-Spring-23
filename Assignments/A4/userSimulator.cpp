#include <bits/stdc++.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "defs.h"
using namespace std;

#define RANDOM_NODE_COUNT 5     // ## Change to 100
#define SLEEP_SECONDS 10        // ## Change to 120

extern vector<vector<int>> graph;
extern map<int, Node> users;
extern my_semaphore shared_sem,logfile_sem,pU_group_sem,rP_group_sem;

my_semaphore write_to_shared(1),read_from_shared(0);

int curr_iter = 0;

void *userSimulator(void *arg){

    while(1){
        
        for(int i=0; i<RANDOM_NODE_COUNT; i++){
            int random_node = rand()%users.size();
            // cout << random_node << " " << users[random_node].degree << " " << users[random_node].log_degree << endl;
            int num_actions = ceil(users[random_node].log_degree);
            
            for(int j=0; j<num_actions; j++){
                int action_type = rand()%3;
                long timestamp = time(0);
                Action action(random_node , ++users[random_node].num_action[action_type] , timestamp , action_type);
                users[random_node].wall.push_back(action);     // Push to Wall queue of user
            }
        }

        write_to_shared._wait();

        curr_iter++;
        cout << endl;
        for(int i=0;i<100;i++) printf("%c" , 'z');
        cout << endl;

        for(int i=0;i<25;i++) read_from_shared._signal();

        sleep(SLEEP_SECONDS);
    }
    pthread_exit(NULL);
}