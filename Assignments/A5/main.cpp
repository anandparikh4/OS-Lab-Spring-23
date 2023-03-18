#include "defs.h"

extern void *guest(void *arg);
extern void *cleaner(void *arg);


set<pair<int,Room>,cmp> rooms;
vector<int> evicted;



int main(){
    srand(time(NULL));
    int n,x,y;
    cout<<"Enter number of rooms: ";
    cin>>n;
    cout<<"Enter number of cleaners: ";
    cin>>x;
    cout<<"Enter number of guests: ";
    cin>>y;

    for(int i=0;i<n;i++){
        Room room(i);
        rooms.insert({0,room});
    }

    pthread_t cleaner_threads[x];
    pthread_t guest_threads[y];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for(int i=0;i<y;i++){
        pthread_create(&guest_threads[i], &attr, guest, (void *)(uintptr_t)i);
    }
    
    for(int i=0;i<x;i++){
        pthread_create(&cleaner_threads[i], &attr, cleaner, (void *)(uintptr_t)i);
    }

    for(int i=0;i<y;i++){
        pthread_join(guest_threads[i], NULL);
    }

    for(int i=0;i<x;i++){
        pthread_join(cleaner_threads[i], NULL);
    }

    return 0;
}