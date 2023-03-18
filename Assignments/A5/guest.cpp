#include "defs.h"

extern set<pair<int,Room>> rooms;
extern vector<int> evicted;

void* guest(void* arg){
    int guest_id = *((int*)arg);
    int sleep_time = gen_rand(GUEST_SLEEP_TIME);
    int stay_time = gen_rand(GUEST_STAY_TIME);
    return NULL;
}