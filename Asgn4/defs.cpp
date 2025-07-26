#include "defs.h"
using namespace std;

void exit_with_error(string s){
    perror(s.c_str());
    exit(0);
}

// Default overloaded constructor
my_semaphore::my_semaphore(int val = 1):
value(val),wakeups(0)
{
    if(pthread_mutex_init(&mutex , NULL) != 0){
        exit_with_error("my_semaphore::pthread_mutex_init() failed");
    }
    if(pthread_cond_init(&cond , NULL) != 0){
        exit_with_error("my_semaphore::pthread_cond_init() failed");
    }
}

// Copy constructor - make a deep copy of all mutex: condition variable and their attributes; copy only the value and wakeups
my_semaphore::my_semaphore(const my_semaphore &s):
value(s.value),wakeups(s.wakeups)
{
    if(pthread_mutex_init(&mutex , NULL) != 0){
        exit_with_error("my_semaphore::pthread_mutex_init() failed");
    }
    if(pthread_cond_init(&cond , NULL) != 0){
        exit_with_error("my_semaphore::pthread_cond_init() failed");
    }
}

// Destructor
my_semaphore::~my_semaphore(){
    if(pthread_mutex_destroy(&mutex) != 0){
        exit_with_error("my_semaphore::pthread_mutex_destroy() failed");
    }
    if(pthread_cond_destroy(&cond) != 0){
        exit_with_error("my_semaphore::pthread_cond_destroy() failed");
    }
}

// semaphore wait
void my_semaphore::_wait(){
    if(pthread_mutex_lock(&mutex) != 0){
        exit_with_error("my_semaphore::_wait::pthread_mutex_lock() failed");
    }
    value--;
    if(value < 0){
        while(wakeups == 0){
            if(pthread_cond_wait(&cond , &mutex) != 0){
                exit_with_error("my_semaphore::_wait::pthread_cond_wait() failed");
            }
        }
        wakeups--;
    }
    if(wakeups > 0){
        cout<<"Broadcasting"<<endl;
        if(pthread_cond_broadcast(&cond) != 0){
            exit_with_error("my_semaphore::_wait::pthread_cond_broadcast() failed");
        }
    } 
    if(pthread_mutex_unlock(&mutex) != 0){
        exit_with_error("my_semaphore::_wait::pthread_mutex_unlock() failed");
    }
}

// semaphore signal
void my_semaphore::_signal(){
    if(pthread_mutex_lock(&mutex) != 0){
        exit_with_error("my_semaphore::pthread_mutex_lock() failed");
    }
    value++;
    if(value <= 0){
        wakeups++;
        if(pthread_cond_broadcast(&cond) != 0){
            exit_with_error("my_semaphore::pthread_cond_broadcast() failed");
        }
    }
    if(pthread_mutex_unlock(&mutex) != 0){
        exit_with_error("my_semaphore::pthread_mutex_unlock() failed");
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

ostream& operator<<(ostream& os, const Action &action)
{
    os << "User ID: " << action.user_id << "| Type: " << action.action_type << "| Action ID: " << action.action_id << "| Timestamp: " <<  action.timestamp;
    return os;
}

// Class Node

// Default constructor
Node::Node(): 
user_id(0)
{
    degree = log_degree = 0;
    sort_by = rand()%2;
    for(int i=0;i<3;i++) num_action[i] = 0;
    wall.clear();
    feed.clear();
    priority.clear();
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
}

// Destructor
Node::~Node()
{
    wall.clear();
    feed.clear();
    priority.clear();
}

void Node::init(){
    log_degree = log2(degree);
    return;
}

void Node::print(){
    cout << "User ID: " << user_id << " Degree: " << degree << " Log Degree: " << log_degree << " Sort By: " << sort_by << " Number of actions: " << num_action[0] << "," << num_action[1] << "," << num_action[2] << std::endl;
}

ostream& operator<<(ostream& os, const Node &node)
{
    os << "User ID: " << node.user_id << "| Degree: " << node.degree << "| Sort By: ";
    if(node.sort_by == 0) os << " Priority";
    else os << " Chronological";
    return os;
}