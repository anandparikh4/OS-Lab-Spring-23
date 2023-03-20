#include "defs.h"

extern void *guest(void *arg);
extern void *cleaner(void *arg);

// ## Add error checking to all syscalls

int N,X,Y;
set<pair<int,Room>,cmp> rooms;
vector<int> evicted;
vector<int> priority;
sem_t hotel_open,hotel_close,cleaner_book,guest_book;
sigset_t sigusr1_set;
pthread_t *guest_threads , *cleaner_threads;

int main(){
    srand(time(NULL));

    signal(SIGUSR1 , sigusr1_handler);

    sigemptyset(&sigusr1_set);
    sigaddset(&sigusr1_set , SIGUSR1);

    cout<<"\nEnter number of rooms: ";
    cin>>N;
    cout<<"\nEnter number of cleaners: ";
    cin>>X;
    cout<<"\nEnter number of guests: ";
    cin>>Y;

    evicted.resize(Y);
    priority.resize(Y);

    for(int i=0;i<N;i++) rooms.insert({0,Room(i)});
    
    for(int i=0;i<Y;i++){
        evicted[i] = 0;
        priority[i] = gen_rand(1,Y);
    }

    // for(const pair<int,Room> &p : rooms){
    //     cout << p.second;
    // }

    sem_init(&hotel_open , 0 , 1);
    sem_init(&hotel_close , 0 , 0);
    sem_init(&cleaner_book , 0 , 1);
    sem_init(&guest_book , 0 , 1);

    guest_threads = (pthread_t *)malloc(Y * sizeof(pthread_t));
    cleaner_threads = (pthread_t *)malloc(X * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for(int i=0;i<Y;i++){
        pthread_create(&guest_threads[i], &attr, guest, (void *)(uintptr_t)i);
    }
    
    for(int i=0;i<X;i++){
        pthread_create(&cleaner_threads[i], &attr, cleaner, (void *)(uintptr_t)i);
    }

    // // main simulating guests
    // for(int i=0;i<3;i++){
    //     sem_wait(&hotel_open);
    //     int x = rand() % 5;
    //     cout << "Main simulates guests for " << x << " seconds" << endl;
    //     for(int i=0;i<N;i++){
    //         Room room(i);
    //         room.tot_duration = rand()%8 + 2;   // [2,9]
    //         cout << room.tot_duration << " " ;
    //         cout << endl;
    //         rooms.insert({0,room});
    //     }
    //     sleep(x);
    //     sem_post(&hotel_close);
    // }

    for(int i=0;i<Y;i++){
        pthread_join(guest_threads[i], NULL);
    }

    for(int i=0;i<X;i++){
        pthread_join(cleaner_threads[i], NULL);
    }

    sem_destroy(&hotel_open);
    sem_destroy(&hotel_close);
    sem_destroy(&cleaner_book);
    sem_destroy(&guest_book);
    free(guest_threads);
    free(cleaner_threads);

    return 0;
}