#ifndef HISTORY_2H
#define HISTORY_2H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <deque>
using namespace std;

class shell_history{
	public:
		deque <char *> dq;
		char *line;
		int history_cnt;
		int history_idx;
		shell_history();
		~shell_history();
		void manage_history();
		void get_history();
};
#endif