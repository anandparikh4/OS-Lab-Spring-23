#include "defs.h"

extern set<pair<int,Room>> rooms;
extern vector<int> evicted;

void* guest(void* arg){
    int guest_id = ((intptr_t)arg);
    cout<<"Guest "<<guest_id<<" created\n";
    int sleep_time = gen_rand(GUEST_SLEEP_TIME);
    int stay_time = gen_rand(GUEST_STAY_TIME);
    pthread_exit(NULL);
}