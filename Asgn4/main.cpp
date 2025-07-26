#include "defs.h"
using namespace std;

extern void * userSimulator(void *);
extern void * pushUpdate(void *);
extern void * readPost(void *);

vector<vector<int>> graph(37700);
vector<Node> users(37700);
ofstream logfile;
my_semaphore write_logfile(1);

// Read from a csv file, edges of a graph, of the form (u,v) in each line and store in a vector of vectors and also intialise a map of users with user_id as key and Node object as value and fill it up with the users in the graph
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
        if(users[u].user_id == 0){
            users[u] = Node(u);
        }
        if(users[v].user_id == 0){
            users[v] = Node(v);
        }
        users[u].degree++;
        users[v].degree++;
    }
    for(int i=0;i<users.size();i++) users[i].init();
    file.close();
}

void precompute_priorities(){
    // int temp = 0;
    set<int> temp;
    for(int i=0;i<users.size();i++){            // each node acts as a common neigbour for any pair of ITS neighbors
        temp.clear();
        for(auto j:graph[i]){
            temp.insert(j);
        }
        for(auto j:graph[i]){
            for(auto k:graph[j]){
                if(temp.find(k) != temp.end())
                    users[i].priority[j]++;
            }
        }
    }
    // cout << temp << endl;
    return;
}

int main(){
    srand(time(NULL));  // seed time only once globally

    load_graph();       // load graph into memory

    ifstream priority_file;
    priority_file.open("priorities.txt",std::ios_base::in);
    if(priority_file.is_open()){
        int u,v,p;
        while(priority_file >> u >> v >> p){
            users[u].priority[v] = p;
        }
    }
    else{
        time_t start_time = time(0),end_time;
        cout<<"Start Compute Time:"<<start_time<<endl;
        precompute_priorities();    // precompute the priorities OF each node FOR each node
        end_time = time(0);
        cout<<"Finish Compute Time:"<<end_time<<endl;
        int max_priority = 0;
        int user_a, user_b;
        ofstream prior_file;
        prior_file.open("priorities.txt",std::ios_base::out);
        for(int i=0;i<users.size();i++){
            for(auto j:users[i].priority){
                if(j.second > max_priority){
                    max_priority = j.second;
                    user_a = i;
                    user_b = j.first;
                }
                prior_file << i << " " << j.first << " " << j.second << "\n";
            }
        }
        prior_file.close();
        cout << "Max Priority: " << max_priority << endl;
        cout << "Users: " << user_a << " " << user_b << endl;
        cout<<"Compute Time:"<<end_time - start_time<<endl;
    }
    // logfile.open("sns.txt", std::ios_base::app);
    // // Select 100 random nodes from the graph
    // vector<int> random_nodes(100);
    // for(int j=0;j<=10;j++){
    //     for(int i=0;i<100;i++){
    //         int temp = rand()%users.size();
    //         random_nodes[i]=temp;
    //     }
    //     int tot_nodes = 0;
    //     set<int> s;
    //     for(int i=0;i<100;i++){
    //         tot_nodes += graph[random_nodes[i]].size();
    //         for(int j=0;j<graph[random_nodes[i]].size();j++){
    //             s.insert(graph[random_nodes[i]][j]);
    //         }
    //     }
    //     logfile<<"--------------------------------------------\n";
    //     logfile << "Total Number of Neighbours: " << tot_nodes << "\n";
    //     logfile << "Total Number of Unique Neighbours: " << s.size() << "\n";
    //     logfile << "Nodes Decreased: " << tot_nodes - s.size() << "\n";
    //     logfile << "Percentage of Nodes Decreased: " << (tot_nodes - s.size())*100.0/tot_nodes << "\n\n\n";
    // }
    // logfile.close();



    logfile.open("sns.log" , std::ios_base::app);

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

    logfile.close();

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