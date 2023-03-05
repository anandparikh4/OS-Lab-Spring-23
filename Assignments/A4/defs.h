#ifndef __DEFS_H
#define __DEFS_H

#include <vector>
#include <string>

void exit_with_error(std::string);

// Class of Actions with action_id, user_id, timestamp, action_type
class Action{
    public:
        int user_id;
        int action_id;
        long timestamp;
        int action_type;

        Action();
        Action(int uid, int aid, int times, int action_t);
        Action(const Action &a);

        ~Action();

        void print();
};

// Class of Node with user_id, degree and log of degree and number of actions and a priority bool
class Node{
    public:
        int user_id;
        int degree;
        int num_actions;
        int priority;
        double log_degree;
        std::vector<Action> wall;
        std::vector<Action> feed;
        
        Node();
        Node(int uid, int deg=0, int actions=0, int pri=0);
        Node(const Node &u);

        ~Node();
        
        void print();
};

#endif