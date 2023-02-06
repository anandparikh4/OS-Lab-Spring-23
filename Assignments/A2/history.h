#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>
using namespace std;

int getch();

void getHistory(vector<string> &history, int &historyIndex, string &currentLine);


#endif