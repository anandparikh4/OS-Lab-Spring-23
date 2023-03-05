#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <errno.h>
#include "defs.h"

using namespace std;

void exit_with_error(string s){
    perror(s.c_str());
    exit(0);
}

void activate(pthread_mutex_t * mutex_t, pthread_mutexattr_t * mutexattr_t, pthread_cond_t * cond_t, pthread_condattr_t * condattr_t){
    if(pthread_mutexattr_init(mutexattr_t) < 0){
        exit_with_error("Node::pthread_mutexattr_init() failed");
    }
    if(pthread_mutex_init(mutex_t , mutexattr_t) < 0){
        exit_with_error("Node::pthread_mutex_init() failed");
    }
    if(pthread_condattr_init(condattr_t) < 0){
        exit_with_error("Node::pthread_condattr_init() failed");
    }
    if(pthread_cond_init(cond_t , condattr_t) < 0){
        exit_with_error("Node::pthread_cond_init() failed");
    }
}

void deactivate(pthread_mutex_t * mutex_t, pthread_mutexattr_t * mutexattr_t, pthread_cond_t * cond_t, pthread_condattr_t * condattr_t){
    if(pthread_mutex_destroy(mutex_t) < 0){
        exit_with_error("~Node::pthread_mutex_destroy() failed");
    }
    if(pthread_mutexattr_destroy(mutexattr_t) < 0){
        exit_with_error("~Node::pthread_mutexattr_destroy() failed");
    }
    if(pthread_cond_destroy(cond_t) < 0){
        exit_with_error("~Node::pthread_cond_destroy() failed");
    }
    if(pthread_condattr_destroy(condattr_t) < 0){
        exit_with_error("~Node::pthread_condattr_destroy() failed");
    }
}

// Class Action 

Action::Action():
user_id(0), action_id(0), timestamp(0), action_type(0)
{}

Action::Action(int uid, int aid, int times, int action_t):
user_id(uid), action_id(aid), timestamp(times), action_type(action_t)
{}

Action::Action(const Action &a):
user_id(a.user_id), action_id(a.action_id), timestamp(a.timestamp), action_type(a.action_type) 
{}

Action::~Action()
{}

void Action::print() {
    cout << "User ID: " << user_id << " Action ID: " << action_id << " Timestamp: " << timestamp << " Action Type: " << action_type << std::endl;
}

// Class Node

// Default constructor
Node::Node(): 
user_id(0), degree(0) , log_degree(0) , sort_by(0)
{
    for(int i=0;i<3;i++) num_action[i] = 0;
    wall.clear();
    feed.clear();
    priority.clear();

    // even the default constructor needs to have this, since destructor will destroy anyways
    activate(&feed_lock , &feed_lock_attr , &feed_cond , &feed_cond_attr);
}

// Overloaded constructor
Node::Node(int uid):
user_id(uid) 
{
    degree = log_degree = 0;
    sort_by = rand()%2;
    for(int i=0;i<3;i++) num_action[i] = 0;
    wall.clear();
    feed.clear();
    priority.clear();

    // initialize pthread mutexes, condition variables and their attributes
    activate(&feed_lock , &feed_lock_attr , &feed_cond , &feed_cond_attr);
}

// Make a full deep copy of everything in the Copy constructor
Node::Node(const Node &u):
user_id(u.user_id), degree(u.degree), log_degree(u.log_degree), sort_by(u.sort_by)
{
    for(int i=0;i<3;i++) num_action[i] = u.num_action[i];
    // The following way of direct copying works for STL containers
    wall = u.wall;
    feed = u.feed;
    priority = u.priority;

    // the pthread mutexes, pthread conditionals (and their attributes) need to be initialized freshly 
    activate(&feed_lock , &feed_lock_attr , &feed_cond , &feed_cond_attr);
}

// Destructor
Node::~Node()
{
    wall.clear();
    feed.clear();
    priority.clear();

    // Destroy pthread mutexes, condition variables and their attributes
    deactivate(&feed_lock , &feed_lock_attr , &feed_cond , &feed_cond_attr);
}

void Node::init(){
    log_degree = log2(degree);
    return;
}

void Node::print(){
    cout << "User ID: " << user_id << " Degree: " << degree << " Log Degree: " << log_degree << " Sort By: " << sort_by << " Number of actions: " << num_action[0] << "," << num_action[1] << "," << num_action[2] << std::endl;
}