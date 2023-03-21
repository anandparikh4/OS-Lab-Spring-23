#include "defs.h"

void exit_with_error(string s){
    perror(s.c_str());
    exit(0);
}

void sigusr1_handler(int sig){
    signal(SIGUSR1 , sigusr1_handler);
    return;
}

void signal_blocker(int sig,int state){
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset , sig);
    if(pthread_sigmask(state , &sigset , NULL) != 0){
        exit_with_error("signal_blocker::pthread_sigmask() failed");
    }
    return;
}

Room::Room(): guest_id(-1), room_id(-1), start_time(0), stay_time(0), tot_duration(0), occupancy(0) {}

Room::Room(int room_num): guest_id(-1), room_id(room_num), start_time(0), stay_time(0), tot_duration(0), occupancy(0) {}

ostream& operator<<(ostream& os, const Room& room){
    os << "Room " << room.room_id << " : " << room.guest_id << " " << room.start_time << " " << room.stay_time << " " << room.tot_duration << " " << room.occupancy << "\n";
    return os;
}

int gen_rand(int min, int max){
    if(max<min) return 0;
    return min + (rand() % (max - min + 1));
}

bool cmp::operator()(const pair<int,Room> &a, const pair<int,Room> &b) const{
    if(a.first == b.first){
        if(a.second.occupancy == b.second.occupancy){
            return a.second.room_id < b.second.room_id;
        }
        return a.second.occupancy < b.second.occupancy;
    }
    return a.first < b.first;
}