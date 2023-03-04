#include <bits/stdc++.h>
// Class of actions with action_id, user_id, timestamp, action_type
class Action {
    public:
    int user_id;
    int action_id;
    long timestamp;
    int action_type;
    Action() : user_id(0), action_id(0), timestamp(0), action_type(0) {}
    Action(int uid, int aid, int times, int action_t) : user_id(uid), action_id(aid), timestamp(times), action_type(action_t) {}
    // Copy constructor
    Action(const Action &a) : user_id(a.user_id), action_id(a.action_id), timestamp(a.timestamp), action_type(a.action_type) {}
    // Print function
    void print() {
        std::cout << "User ID: " << user_id << " Action ID: " << action_id << " Timestamp: " << timestamp << " Action Type: " << action_type << std::endl;
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
    std::vector<Action> wall;
    std::vector<Action> feed;
    Node() : user_id(0), degree(0), num_actions(0), priority(0), log_degree(0) {}

    Node(int uid, int deg=0, int actions=0, int pri=0) : user_id(uid), degree(deg), num_actions(actions), priority(pri) {
        if(degree == 0) {
            log_degree = 0;
        }
        else
            log_degree = log2(degree);
    }

    Node(const Node &u) : user_id(u.user_id), degree(u.degree), num_actions(u.num_actions), priority(u.priority), log_degree(u.log_degree) {}
    // Print function
    void print() {
        std::cout << "User ID: " << user_id << " Degree: " << degree << " Log Degree: " << log_degree << " Num Actions: " << num_actions << " Priority: " << priority << std::endl;
    }
};