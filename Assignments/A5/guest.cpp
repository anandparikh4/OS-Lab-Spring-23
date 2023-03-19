#include "defs.h"

extern int N;
extern set<pair<int,Room>,cmp> rooms;
extern vector<int> evicted;
extern sem_t hotel_open,hotel_close,guest_book;

void* guest(void* arg){
    int guest_id = ((intptr_t)arg);
    cout<<"Guest "<<guest_id<<" created\n";
    int sleep_time = gen_rand(GUEST_SLEEP_TIME);
    int stay_time = gen_rand(GUEST_STAY_TIME);
    pthread_exit(NULL);
}