#include "defs.h"

extern void *guest(void *arg);
extern void *cleaner(void *arg);

int N,X,Y;
set<pair<int,Room>,cmp> rooms;
vector<int> evicted;
vector<int> priority;
sem_t hotel_open,hotel_close,cleaner_book,guest_book;
sigset_t sigusr1_set;
pthread_t *guest_threads , *cleaner_threads;

int main(){
    int seed = time(NULL);
    srand(seed);

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

    if(sem_init(&hotel_open , 0 , 1) < 0){
        exit_with_error("sem_init() failed");
    }
    if(sem_init(&hotel_close , 0 , 0) < 0){
        exit_with_error("sem_init() failed");
    }
    if(sem_init(&cleaner_book , 0 , 1) < 0){
        exit_with_error("sem_init() failed");
    }
    if(sem_init(&guest_book , 0 , 1) < 0){
        exit_with_error("sem_init() failed");
    }

    guest_threads = (pthread_t *)malloc(Y * sizeof(pthread_t));
    cleaner_threads = (pthread_t *)malloc(X * sizeof(pthread_t));
    pthread_attr_t attr;
    if(pthread_attr_init(&attr) != 0){
        exit_with_error("pthread_attr_init() failed");
    }

    for(int i=0;i<Y;i++){
        if(pthread_create(&guest_threads[i], &attr, guest, (void *)(uintptr_t)i) != 0){
            exit_with_error("pthread_create() failed");
        }
    }
    
    for(int i=0;i<X;i++){
        if(pthread_create(&cleaner_threads[i], &attr, cleaner, (void *)(uintptr_t)i) != 0){
            exit_with_error("pthread_create() failed");
        }
    }

    for(int i=0;i<Y;i++){
        if(pthread_join(guest_threads[i], NULL) != 0){
            exit_with_error("pthread_join() failed");
        }
    }

    for(int i=0;i<X;i++){
        if(pthread_join(cleaner_threads[i], NULL) != 0){
            exit_with_error("pthread_join() failed");
        }
    }

    if(sem_destroy(&hotel_open) < 0){
        exit_with_error("sem_destroy() failed");
    }
    if(sem_destroy(&hotel_close) < 0){
        exit_with_error("sem_destroy() failed");
    }
    if(sem_destroy(&cleaner_book) < 0){
        exit_with_error("sem_destroy() failed");
    }
    if(sem_destroy(&guest_book) < 0){
        exit_with_error("sem_destroy() failed");
    }
    free(guest_threads);
    free(cleaner_threads);

    return 0;
}