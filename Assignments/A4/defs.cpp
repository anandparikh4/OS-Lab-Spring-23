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

Node::Node(): 
user_id(0), degree(0), num_actions(0), priority(0), log_degree(0)
{}

Node::Node(int uid, int deg, int actions, int pri): 
user_id(uid), degree(deg), num_actions(actions), priority(pri) 
{
    if(degree == 0) log_degree = 0;
    else log_degree = log2(degree);
}

Node::Node(const Node &u):
user_id(u.user_id), degree(u.degree), num_actions(u.num_actions), priority(u.priority), log_degree(u.log_degree) 
{}

Node::~Node()
{}

void Node::init(){
    log_degree = log2(degree);
    return;
}

void Node::print(){
    cout << "User ID: " << user_id << " Degree: " << degree << " Log Degree: " << log_degree << " Num Actions: " << num_actions << " Priority: " << priority << std::endl;
}