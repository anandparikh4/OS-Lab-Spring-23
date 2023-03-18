#include "defs.h"



ostream& operator<<(ostream& os, const Room& room){
    os << "Room " << room.room_id << " : " << room.guest_id << " " << room.start_time << " " << room.tot_duration << " " << room.occupancy << "\n";
    return os;
}

int gen_rand(int min, int max){
    if(max<min) return 0;
    return min + (rand() % (max - min + 1));
}

bool cmp::operator()(const pair<int,Room> &a, const pair<int,Room> &b){
    if(a.first == b.first){
        return a.second.occupancy < b.second.occupancy;
    }
    return a.first < b.first;
}