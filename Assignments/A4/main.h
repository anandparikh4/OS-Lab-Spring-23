#include <bits/stdc++.h>
// Class of actions with action_id, user_id, timestamp, action_type
class Action {
    public:
        int user_id;
        int action_id;
        long timestamp;
        int action_type;
        Action(int user_id, int action_id, int timestamp, int action_type) {
            this->user_id = user_id;
            this->action_id = action_id;
            this->timestamp = timestamp;
            this->action_type = action_type;
        }
};

// Class of User node with user_id, degree and log of degree and number of actions and a priority bool
class User {
    public:
        int user_id;
        int degree;
        int num_actions;
        bool priority;
        double log_degree;

        User(int user_id, int degree=0, int num_actions=0, bool priority=0) {
            this->user_id = user_id;
            this->degree = degree;
            // Calculate log base 2 of this->degree
            this->log_degree = log2(this->degree);
            this->num_actions = num_actions;
            this->priority = priority;
        }
        // Copy constructor
        User(const User &u) {
            this->user_id = u.user_id;
            this->degree = u.degree;
            this->log_degree = u.log_degree;
            this->num_actions = u.num_actions;
            this->priority = u.priority;
        }
};