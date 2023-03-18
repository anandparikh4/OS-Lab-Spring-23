#include <bits/stdc++.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

#define GUEST_SLEEP_TIME 10,20
#define GUEST_STAY_TIME 10,30

const int infinity = 1e9+7; 

using namespace std;

class Room{
    public:
    int guest_id;
    int room_id;  // Can put it, as it is intialised in main and all threads just change other parameters
    int start_time;
    int tot_duration;
    int occupancy;

    Room(): guest_id(-1), room_id(-1), start_time(-1), tot_duration(0), occupancy(0) {}
    
    Room(int room_num): room_id(room_num), guest_id(-1), start_time(-1), tot_duration(0), occupancy(0) {}

    ~Room() {}

    friend ostream& operator<<(ostream& os, const Room& room);

};

int gen_rand(int min, int max);