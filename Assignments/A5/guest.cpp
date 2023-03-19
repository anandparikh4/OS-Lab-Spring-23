#include "defs.h"

extern int N;
extern set<pair<int,Room>,cmp> rooms;
extern vector<int> evicted,priority;
extern sem_t hotel_open,hotel_close,guest_book;

/*
    int guest_id;
    int room_id;  // Can put it, as it is intialised in main and all threads just change other parameters
    int start_time;
    int tot_duration;
    int occupancy;
*/

int guest_start = 0,guest_finish = 0;

void* guest(void* arg){
    int guest_id = ((intptr_t)arg);
    cout<<"Guest "<<guest_id<<" created\n";

    while(1){
        int sleep_time = gen_rand(GUEST_SLEEP_TIME);
        int stay_time = gen_rand(GUEST_STAY_TIME);

        sem_wait(&guest_book);
        if(guest_start == 0){
            sem_wait(&hotel_open);
        }
        auto it = rooms.begin();

        if(rooms.empty() || it->first >= priority[guest_id]){   // either all rooms currently occupied for the 2nd time OR all rooms occupied and minimum priority guest has higher priority than guest_id
            sem_post(&guest_book);
            cout << "Guest ID: " << guest_id << " is refused a room" << endl;
            sleep(sleep_time);
            continue;
        }

        Room room(-1);
        int new_priority = -1;

        if(it->first == 0){     // empty room available
            room.occupancy = it->second.occupancy + 1;
            if(room.occupancy == 2) new_priority = INF;
            else new_priority = priority[guest_id];
            room.start_time = time(NULL);
            room.guest_id = guest_id;
            rooms.erase(it);
            rooms.insert({new_priority , room});
            cout << "Guest ID: " << guest_id << " starts their stay in room ID: " << room.room_id;
        }
        
        else{       // no empty room, but can evict another guest
            room.occupancy = 2;
            new_priority = INF;
            int curr_time = time(NULL);
            room.tot_duration += curr_time - it->second.start_time;
            room.start_time = curr_time;
            room.guest_id = guest_id;
            evicted[it->second.guest_id] = room.start_time;     // to notify the evicted person when they were evicted
            pthread_kill(it->second.guest_id , SIGUSR1);
            cout << "Guest ID: " << guest_id << " evicts Guest ID: " << it->second.guest_id << " from room ID: " << room.room_id;
            rooms.erase(it);
            rooms.insert({new_priority , room});
        }

        signal_blocker(SIGUSR1 , SIG_BLOCK);
        signal_blocker(SIGALRM , SIG_BLOCK);

        guest_start++;
        sem_post(&guest_book);

        alarm(stay_time);

        sigset_t empty_set;
        sigemptyset(&empty_set);
        sigsuspend(&empty_set);

        signal_blocker(SIGUSR1 , SIG_UNBLOCK);
        signal_blocker(SIGALRM , SIG_UNBLOCK);

        sem_wait(&guest_book);
        guest_finish++;
        if(evicted[guest_id]){
            cout << "Guest ID: " << guest_id << " realizes they were evicted after " << evicted[guest_id] - room.start_time << " seconds" << endl;
        }
        else{
            auto it = rooms.find({new_priority , room});
            rooms.erase(it);
            int curr_time = time(NULL);
            room.tot_duration += curr_time - room.start_time;
            if(room.occupancy == 2) new_priority = INF;
            else new_priority = 0;
            rooms.insert({new_priority , room});
            cout << "Guest ID: " << guest_id << " finishes their stay in room ID: " << room.room_id << " of " << curr_time - room.start_time << " seconds" << endl;
        }
        if(guest_finish == 2 * N){
            guest_start = 0;
            guest_finish = 0;
            sem_post(&hotel_close);
        }
        sem_post(&guest_book);

        sleep(sleep_time);
    }

    pthread_exit(NULL);
}