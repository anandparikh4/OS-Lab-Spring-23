#ifndef __DEFS_H
#define __DEFS_H

#include <vector>
#include <map>
#include <string>

void exit_with_error(std::string);

// instead of using scattered global objects, put them in a struct, making it easier to handle
struct global_lock{
    // shared queue mutex, condition variable and their attributes
    pthread_mutex_t shared_lock;
    pthread_mutexattr_t shared_lock_attr;
    pthread_cond_t shared_cond;
    pthread_condattr_t shared_cond_attr;

    // log file mutex, condition variable and their attributes
    pthread_mutex_t logfile_lock;
    pthread_mutexattr_t logfile_lock_attr;
    pthread_cond_t logfile_cond;
    pthread_condattr_t logfile_cond_attr;
};

void activate(pthread_mutex_t * , pthread_mutexattr_t * , pthread_cond_t * , pthread_condattr_t *);

void deactivate(pthread_mutex_t * , pthread_mutexattr_t * , pthread_cond_t * , pthread_condattr_t *);

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

        // the lock to wait on for accessing this Node's feed queue
        pthread_mutex_t feed_lock;
        pthread_mutexattr_t feed_lock_attr;
        // the condition to wait on to participate in contention for this Node's lock
        pthread_cond_t feed_cond;
        pthread_condattr_t feed_cond_attr;

        Node();
        Node(int uid);
        Node(const Node &u);

        ~Node();
        
        void init();
        void print();
};

#endif