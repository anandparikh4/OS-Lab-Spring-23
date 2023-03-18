#include "defs.h"

extern set<pair<int,Room>> rooms;
extern vector<int> evicted;

void* cleaner(void* arg){
    int cleaner_id = *((int*)arg);
    
    return NULL;
}