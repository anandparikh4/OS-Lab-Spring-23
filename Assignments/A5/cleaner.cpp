#include "defs.h"

extern set<pair<int,Room>> rooms;
extern vector<int> evicted;

void* cleaner(void* arg){
    int cleaner_id = ((intptr_t)arg);
    cout<<"Cleaner "<<cleaner_id<<" created\n";
    pthread_exit(NULL);
}