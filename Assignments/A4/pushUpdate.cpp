#include <bits/stdc++.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "defs.h"
using namespace std;

void * pushUpdate(void * param){
    int id = (intptr_t)param;
    cout << id << endl;
    pthread_exit(0);
}