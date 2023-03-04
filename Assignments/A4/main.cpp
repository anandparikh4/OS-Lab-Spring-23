#include <bits/stdc++.h>
using namespace std;

// Class of actions with action_id, user_id, timestamp, action_type
class Action {
    public:
        int user_id;
        int action_id;
        long timestamp;
        int action_type;
        Action(int user_id, int action_id, int timestamp, int action_type) {
            user_id = user_id;
            action_id = action_id;
            timestamp = timestamp;
            action_type = action_type;
        }
};

// Class of Node with user_id, degree and log of degree and number of actions and a priority bool
class Node {
    public:
        int user_id;
        int degree;
        int num_actions;
        int priority;
        double log_degree;

        Node(int user_id, int degree=0, int num_actions=0, int priority=0) {
            user_id = user_id;
            degree = degree;
            // Calculate log base 2 of this->degree
            log_degree = log2(this->degree);
            num_actions = num_actions;
            priority = priority;
        }
        // Copy constructor
        Node(const Node &u) {
            user_id = u.user_id;
            degree = u.degree;
            log_degree = u.log_degree;
            num_actions = u.num_actions;
            priority = u.priority;
        }
};

// Read from an csv file, edges of a graphy, of the form (u,v) in each line and store in a vector of vectors and also intialise a map of users with user_id as key and Node object as value and fill it up with the users in the graph
void read_graph(vector<vector<int>> &graph, map<int, Node> &users) {
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
    // Vector of vectors to store the graph
    vector<vector<int>> graph(38000);
    // Map of users with user_id as key and Node object as value
    map<int, Node> users;
    // Read the graph from the csv file
    read_graph(graph, users);
    // Print the number of users in the graph
    cout << "Number of users in the graph: " << users.size() << endl;
    // Print the number of edges in the graph
    int num_edges = 0;
    for(int i=0; i<graph.size(); i++) {
        num_edges += graph[i].size();
    }
    cout << "Number of edges in the graph: " << num_edges/2 << endl;
    



    return 0;
}
            