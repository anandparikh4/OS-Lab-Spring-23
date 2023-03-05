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

void Action::print() {
    cout << "User ID: " << user_id << " Action ID: " << action_id << " Timestamp: " << timestamp << " Action Type: " << action_type << std::endl;
}

void Node::print(){
    cout << "User ID: " << user_id << " Degree: " << degree << " Log Degree: " << log_degree << " Num Actions: " << num_actions << " Priority: " << priority << std::endl;
}