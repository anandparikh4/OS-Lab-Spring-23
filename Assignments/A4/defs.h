#ifndef __DEFS_H
#define __DEFS_H

#include <vector>
#include <map>
#include <string>
#include <pthread.h>
#include <iostream>
#include <fstream>

#define PUSHUPDATE_THREAD_COUNT 5   // ## Change to 25
#define READPOST_THREAD_COUNT 10
#define RANDOM_NODE_COUNT 10         // ## Change to 100
#define SLEEP_SECONDS 10        // ## Change to 120
#define ACTIONS_PROPORTIONALITY_CONSTANT 1

void exit_with_error(std::string);

class my_semaphore{
    private:
        int value, wakeups;
        pthread_mutex_t mutex;
        pthread_mutexattr_t mutexattr;
        pthread_cond_t cond;
        pthread_condattr_t condattr;

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