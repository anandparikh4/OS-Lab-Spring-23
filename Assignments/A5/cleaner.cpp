#include "defs.h"

extern int N;
extern set<pair<int,Room>,cmp> rooms;
extern vector<int> evicted;
extern sem_t hotel_open,hotel_close,cleaner_book;

int cleaner_start = 0,cleaner_finish = 0;

void* cleaner(void* arg){
    int cleaner_id = ((intptr_t)arg);
    cout<<"Cleaner "<<cleaner_id<<" created\n";

    while(1){
        sem_wait(&cleaner_book);
        if(cleaner_start == 0){
            sem_wait(&hotel_close);
        }
        if(rooms.empty()){
            sem_post(&cleaner_book);
            continue;
        }
        // always pick the first room, which is statistically randomized
        auto it = rooms.begin();
        // for(int i=0;i<rand() % rooms.size();i++) it++;  // pick a random room
        Room room = it->second;
        rooms.erase(it);
        cleaner_start++;
        sem_post(&cleaner_book);

        sleep(PROPORTIONALITY_CONSTANT * room.tot_duration);

        sem_wait(&cleaner_book);
        cout << "Cleaner ID: " << cleaner_id << " cleans room ID: " << room.room_id << " for " << PROPORTIONALITY_CONSTANT * room.tot_duration << " seconds" << endl;
        cleaner_finish++;
        if(cleaner_finish == N){
            cleaner_start = 0;
            cleaner_finish = 0;
            for(int i=0;i<N;i++){
                rooms.insert({0,Room(i)});
            }
            sem_post(&hotel_open);
        }
        sem_post(&cleaner_book); 
    }

    pthread_exit(NULL);
}