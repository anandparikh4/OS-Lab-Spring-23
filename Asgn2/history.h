#ifndef HISTORY_2H
#define HISTORY_2H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <deque>
using namespace std;

#define MAX_COMMANDS 1000

class shell_history{
	public:
		deque <char *> dq;
		char *line;
		int history_cnt;
		int history_idx;
		char history_file[256];
		char latest_command[4096];
		shell_history();
		~shell_history();
		void manage_history();
		void get_history();
};
#endif