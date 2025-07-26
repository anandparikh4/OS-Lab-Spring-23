#include "defs.h"

extern int N;
extern set<pair<int,Room>,cmp> rooms;
extern vector<int> evicted,priority;
extern sem_t hotel_open,hotel_close,guest_book;
extern sigset_t sigusr1_set;
extern pthread_t *guest_threads;

int guest_start = 0;

void* guest(void* arg){
    int guest_id = ((intptr_t)arg);

    while(1){
        int sleep_time = gen_rand(GUEST_SLEEP_TIME);
        int stay_time = gen_rand(GUEST_STAY_TIME);

        sleep(sleep_time);

        if(sem_wait(&guest_book) < 0){
            exit_with_error("guest::sem_wait() failed");
        }
        if(guest_start == 0){
            if(sem_wait(&hotel_open) < 0){
                exit_with_error("guest::sem_wait() failed");
            }
        }
        auto it = rooms.begin();

        if(rooms.empty() || it->first >= priority[guest_id]){   // either all rooms currently occupied for the 2nd time OR all rooms occupied and minimum priority guest has higher priority than guest_id
            // cout << "Guest ID: " << guest_id << " is refused a room" << endl;
            if(sem_post(&guest_book) < 0){
                exit_with_error("guest::sem_post() failed");
            }
            continue;
        }

        Room room(-1);
        int new_priority = -1;

        if(it->first == 0){     // empty room available
            room.guest_id = guest_id;
            room.room_id = it->second.room_id;
            room.start_time = time(NULL);
            room.stay_time = stay_time;
            room.tot_duration = it->second.tot_duration;
            room.occupancy = it->second.occupancy + 1;
            if(room.occupancy == 2) new_priority = INF;
            else new_priority = priority[guest_id];
            rooms.erase(it);
            rooms.insert({new_priority , room});
            cout << "Guest ID-" << guest_id << " starts their stay in room ID-" << room.room_id << " for " << stay_time << " seconds" << endl;
        }
        
        else{       // no empty room, but can evict another guest
            room.guest_id = guest_id;
            room.room_id = it->second.room_id;
            int curr_time = time(NULL);
            room.start_time = curr_time;
            room.tot_duration = min(curr_time - it->second.start_time,room.stay_time);
            room.stay_time = stay_time;
            room.occupancy = 2;
            new_priority = INF;
            if(it->second.guest_id>=0){
                evicted[it->second.guest_id] = curr_time;     // to notify the evicted person when they were evicted
                if(pthread_kill(guest_threads[it->second.guest_id] , SIGUSR1) != 0){
                    exit_with_error("guest::pthread_kill() failed");
                }
            }
            cout << "Guest ID-" << guest_id << " evicts Guest ID-" << it->second.guest_id << " from room ID-" << room.room_id << endl;
            cout << "Guest ID-" << guest_id << " starts their stay in room ID-" << room.room_id << " for " << stay_time << " seconds" << endl;
            rooms.erase(it);
            rooms.insert({new_priority , room});
        }

        guest_start++;
        // If (2*N)th guest arrives, then close the hotel and let the cleaners clean the rooms
        if(guest_start == 2 * N){
            guest_start = 0;
            if(sem_post(&hotel_close) < 0){
                exit_with_error("guest::sem_post() failed");
            }
            if(sem_post(&guest_book) < 0){
                exit_with_error("guest::sem_post() failed");
            }
            continue;
        }

        signal_blocker(SIGUSR1 , SIG_BLOCK);

        if(sem_post(&guest_book) < 0){
            exit_with_error("guest::sem_post() failed");
        }

        timespec ts;
        ts.tv_sec = stay_time;
        ts.tv_nsec = 0;
        sigtimedwait(&sigusr1_set , NULL , &ts);

        if(sem_wait(&guest_book) < 0){
            exit_with_error("guest::sem_wait() failed");
        }

        signal_blocker(SIGUSR1 , SIG_UNBLOCK);

        if(guest_start==0){
            if(sem_post(&guest_book) < 0){
                exit_with_error("guest::sem_post() failed");
            }
            continue;
        }

        if(evicted[guest_id]){
            cout << "Guest ID-" << guest_id << " realizes they were evicted from room ID-" << room.room_id << " after " << evicted[guest_id] - room.start_time << " seconds" << endl;
            evicted[guest_id] = 0;
        }
        else{
            auto it = rooms.find({new_priority , room});
            if(it == rooms.end()){
                if(sem_post(&guest_book) < 0){
                    exit_with_error("guest::sem_post() failed");
                }
                continue;
            }
            rooms.erase(it);
            int curr_time = time(NULL);
            room.tot_duration += min((curr_time - room.start_time),room.stay_time);
            room.guest_id = -1;
            room.start_time = -1;
            room.stay_time = 0;
            if(room.occupancy == 2) new_priority = INF;
            else new_priority = 0;
            rooms.insert({new_priority , room});
            cout << "Guest ID-" << guest_id << " finishes their stay in room ID-" << room.room_id << endl;
        }
        if(sem_post(&guest_book) < 0){
            exit_with_error("guest::sem_post() failed");
        }

    }

    pthread_exit(NULL);
}