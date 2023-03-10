#ifndef __DEFS_H
#define __DEFS_H

#include <vector>
#include <map>
#include <string>
#include <pthread.h>
#include <iostream>
#include <fstream>

const int PUSHUPDATE_THREAD_COUNT = 25;              // ## Change to 25
const int READPOST_THREAD_COUNT = 10;               // ## Change to 10
const int RANDOM_NODE_COUNT = 100;                   // ## Change to 100
const int SLEEP_SECONDS = 120;                       // ## Change to 120
const int ACTIONS_PROPORTIONALITY_CONSTANT = 10;     // ## Change to 10 
const int NUM_BATCHES = 4;                          // ## Change to 4
const int BATCH_SIZE = RANDOM_NODE_COUNT/NUM_BATCHES;

void exit_with_error(std::string);

class my_semaphore{
    private:
        int value, wakeups;
        pthread_mutex_t mutex;
        pthread_cond_t cond;
        // using default (NULL) attributes for pthread_mutexattr_t and pthread_condattr_t

    public:
        my_semaphore(int val);
        my_semaphore(const my_semaphore &s);

        ~my_semaphore();

        void _wait();

        void _signal();
};

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
        friend std::ostream& operator<<(std::ostream& os, const Action &action);
};

class Node{
    public:
        int user_id;
        int degree;
        int sort_by;
        double log_degree;
        int num_action[3];
        std::map<int,int> priority;
        std::vector<Action> wall;
        std::vector<Action> feed;

        my_semaphore feedsem;

        Node();
        Node(int uid);
        Node(const Node &u);

        ~Node();
        
        void init();
        void print();
        friend std::ostream& operator<<(std::ostream& os, const Node &node);
};

#endif