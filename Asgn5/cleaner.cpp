#include "defs.h"

extern int N,Y;
extern set<pair<int,Room>,cmp> rooms;
extern vector<int> evicted;
extern sem_t hotel_open,hotel_close,cleaner_book;
extern pthread_t *guest_threads;

int cleaner_start = 0,cleaner_finish = 0;
int curr_time = -1;
void* cleaner(void* arg){
    int cleaner_id = ((intptr_t)arg);
    // cout<<"Cleaner "<<cleaner_id<<" created\n";

    while(1){
        if(sem_wait(&cleaner_book) < 0){
            exit_with_error("cleaner::sem_wait() failed");
        }
        if(cleaner_start == 0){
            if(sem_wait(&hotel_close) < 0){
                exit_with_error("cleaner::sem_wait() failed");
            }
            if(curr_time==-1)   curr_time = (int)time(NULL);
        }
        cleaner_start++;
        if(rooms.empty()){
            if(sem_post(&cleaner_book) < 0){
                exit_with_error("cleaner::sem_post() failed");
            }
            continue;
        }
        // always pick the first room, which is statistically randomized
        auto it = rooms.begin();
        int priority = it->first;
        Room room = it->second;
        rooms.erase(it);
        // Evict the guest if present
        if(priority > 0){
            int temp_stay_time = (curr_time- room.start_time);
            if(temp_stay_time<0){
                temp_stay_time = (time(NULL) - room.start_time);
            }
            if(temp_stay_time < room.stay_time){
                cout<< "Cleaner ID-" << cleaner_id << " evicts guest ID-" << room.guest_id << " from room ID-" << room.room_id << " after " << temp_stay_time << " seconds of their stay" << endl;
            }
            room.tot_duration += min(temp_stay_time,room.stay_time);
            if(room.guest_id>=0){
                if(pthread_kill(guest_threads[room.guest_id] , SIGUSR1) != 0){
                    exit_with_error("guest::pthread_kill() failed");
                }
            }
        }
        cout << "Cleaner ID-" << cleaner_id << " starts cleaning room ID-" << room.room_id << " for " << PROPORTIONALITY_CONSTANT * room.tot_duration << " seconds" << endl;
        if(sem_post(&cleaner_book) < 0){
            exit_with_error("cleaner::sem_post() failed");
        }

        sleep(PROPORTIONALITY_CONSTANT * room.tot_duration);

        if(sem_wait(&cleaner_book) < 0){
            exit_with_error("cleaner::sem_wait() failed");
        }
        cout << "Cleaner ID-" << cleaner_id << " finishes cleaning room ID-" << room.room_id << endl;
        cleaner_finish++;
        if(cleaner_finish == N){
            cleaner_start = 0;
            cleaner_finish = 0;
            curr_time = -1;
            rooms.clear();
            for(int i=0;i<N;i++) rooms.insert({0,Room(i)});
            for(int i=0;i<Y;i++) evicted[i] = 0;
            if(sem_post(&hotel_open) < 0){
                exit_with_error("cleaner::sem_post() failed");
            }   
        }
        if(sem_post(&cleaner_book) < 0){
            exit_with_error("cleaner::sem_post() failed");
        }
    }

    pthread_exit(NULL);
}