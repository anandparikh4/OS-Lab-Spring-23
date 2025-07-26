#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <deque>
#include <unistd.h>
#include <termios.h>
using namespace std;

#define MAX_COMMANDS 1000

int getch();

void getHistory(deque<string> &history, int &historyIndex, string &currentLine);


#endif