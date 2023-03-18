#include "defs.h"


int gen_rand(int min, int max){
    if(max<min) return 0;
    return min + (rand() % (max - min + 1));
}

ostream& operator<<(ostream& os, const Room& room){
    os << "Room " << room.room_id << " : " << room.guest_id << " " << room.start_time << " " << room.tot_duration << " " << room.occupancy << "\n";
    return os;
}