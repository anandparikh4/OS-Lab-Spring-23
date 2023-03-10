#include "defs.h"
using namespace std;

extern vector<vector<int>> graph;
extern vector<Node> users;
extern my_semaphore write_logfile;
extern ofstream logfile;

vector<vector<Action> * > shared(BATCH_SIZE);
my_semaphore write_shared(1),read_shared(0);
int curr_uS_iter = 0;


void *userSimulator(void *arg){

    while(1){
        for(int batch = 0;batch<NUM_BATCHES;batch++){        
            vector<vector<Action>> static_copy(BATCH_SIZE);
            vector<vector<Action> * > dynamic_copy(BATCH_SIZE);

            for(int i=0; i<BATCH_SIZE; i++){
                int random_node = rand()%users.size();
                // cout << random_node << " " << users[random_node].degree << " " << users[random_node].log_degree << endl;
                int num_actions = ACTIONS_PROPORTIONALITY_CONSTANT*floor(1+users[random_node].log_degree);
                dynamic_copy[i] = new vector<Action>;
                
                for(int j=0; j<num_actions; j++){
                    int action_type = rand()%3;
                    long timestamp = time(0);
                    Action action(random_node , ++users[random_node].num_action[action_type] , timestamp , action_type);
                    users[random_node].wall.push_back(action);     // Push to Wall queue of user
                    static_copy[i].push_back(action);
                    dynamic_copy[i]->push_back(action);
                }
                // static_copy[i].first = static_copy[i].second.size() * graph[random_node].size();
            }

            // sort(static_copy.begin() , static_copy.end() , cmp);        // sort by decreasing number of total number of pushes

            write_shared._wait();
            // write to shared
            curr_uS_iter++;
            //Minimum number of actions in a node's wall
            // cout<<"Minimum number of actions in a node's wall : "<<static_copy[BATCH_SIZE-1].second.size()<<endl;
            

            for(int i=0;i<BATCH_SIZE;i++){
                shared[(i+curr_uS_iter)%BATCH_SIZE] = dynamic_copy[i];    // round-robin load balancing        
            }
            read_shared._signal();

            write_logfile._wait();
            // write to log file
            logfile << "userSimulator iteration #" << curr_uS_iter << " : " << "\n";
            for(int i=0;i<BATCH_SIZE;i++){
                logfile<<"Node "<<i<<" : "<<"\n";
                for(int j=0;j<static_copy[i].size();j++) logfile << static_copy[i][j] << "\n";
            }
            logfile << "---------------------------------------------------------------------------"<<endl;
            cout << "Completed: userSimulator: iteration #" << curr_uS_iter<< endl;
            write_logfile._signal();

            // write_stdout._wait();
            // write_stdout._signal();
        }

        sleep(SLEEP_SECONDS);
    }

    pthread_exit(NULL);
}