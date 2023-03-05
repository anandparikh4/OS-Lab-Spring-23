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
    if(pthread_mutexattr_init(&feed_lock_attr) < 0){
        exit_with_error("Node::pthread_mutexattr_init() failed");
    }
    if(pthread_mutex_init(&feed_lock , &feed_lock_attr) < 0){
        exit_with_error("Node::pthread_mutex_init() failed");
    }
    if(pthread_condattr_init(&feed_cond_attr) < 0){
        exit_with_error("Node::pthread_condattr_init() failed");
    }
    if(pthread_cond_init(&feed_cond , &feed_cond_attr) < 0){
        exit_with_error("Node::pthread_cond_init() failed");
    }
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
    if(pthread_mutexattr_init(&feed_lock_attr) < 0){
        exit_with_error("Node::pthread_mutexattr_init() failed");
    }
    if(pthread_mutex_init(&feed_lock , &feed_lock_attr) < 0){
        exit_with_error("Node::pthread_mutex_init() failed");
    }
    if(pthread_condattr_init(&feed_cond_attr) < 0){
        exit_with_error("Node::pthread_condattr_init() failed");
    }
    if(pthread_cond_init(&feed_cond , &feed_cond_attr) < 0){
        exit_with_error("Node::pthread_cond_init() failed");
    }
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
    if(pthread_mutexattr_init(&feed_lock_attr) < 0){
        exit_with_error("Node::pthread_mutexattr_init() failed");
    }
    if(pthread_mutex_init(&feed_lock , &feed_lock_attr) < 0){
        exit_with_error("Node::pthread_mutex_init() failed");
    }
    if(pthread_condattr_init(&feed_cond_attr) < 0){
        exit_with_error("Node::pthread_condattr_init() failed");
    }
    if(pthread_cond_init(&feed_cond , &feed_cond_attr) < 0){
        exit_with_error("Node::pthread_cond_init() failed");
    }
}

// Destructor
Node::~Node()
{
    wall.clear();
    feed.clear();
    priority.clear();

    // Destroy pthread mutexes, condition variables and their attributes
    if(pthread_mutex_destroy(&feed_lock) < 0){
        exit_with_error("~Node::pthread_mutex_destroy() failed");
    }
    if(pthread_mutexattr_destroy(&feed_lock_attr) < 0){
        exit_with_error("~Node::pthread_mutexattr_destroy() failed");
    }
    if(pthread_cond_destroy(&feed_cond) < 0){
        exit_with_error("~Node::pthread_cond_destroy() failed");
    }
    if(pthread_condattr_destroy(&feed_cond_attr) < 0){
        exit_with_error("~Node::pthread_condattr_destroy() failed");
    }
}

void Node::init(){
    log_degree = log2(degree);
    return;
}

void Node::print(){
    cout << "User ID: " << user_id << " Degree: " << degree << " Log Degree: " << log_degree << " Sort By: " << sort_by << " Number of actions: " << num_action[0] << "," << num_action[1] << "," << num_action[2] << std::endl;
}