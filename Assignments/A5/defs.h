#include <bits/stdc++.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

#define GUEST_SLEEP_TIME 10,20
#define GUEST_STAY_TIME 10,30

const int PROPORTIONALITY_CONSTANT = 1;
const int INF = 1e9+7; 

using namespace std;

void sigusr1_handler(int sig);

void sigalrm_handler(int sig);

void signal_blocker(int sig,int state);

class Room{
    public:
    int guest_id;
    int room_id;  // Can put it, as it is intialised in main and all threads just change other parameters
    int start_time;
    int tot_duration;
    int occupancy;

    Room(): guest_id(-1), room_id(-1), start_time(-1), tot_duration(0), occupancy(0) {}
    
    Room(int room_num): room_id(room_num), guest_id(-1), start_time(-1), tot_duration(0), occupancy(0) {}

    Room(const Room & R): room_id(R.room_id) , guest_id(R.guest_id) , start_time(R.start_time) , tot_duration(R.tot_duration) , occupancy(R.occupancy) {}

    ~Room() {}

    friend ostream& operator<<(ostream& os, const Room& room);
};

int gen_rand(int min, int max);

struct cmp {
    // Operator() overloading
    bool operator()(const pair<int,Room> &a, const pair<int,Room> &b) const;
};