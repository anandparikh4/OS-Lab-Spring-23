#include <unistd.h>
#include <sys/shm.h>

int main(){
    int a,b,c,d,e;

    key_t w = ftok("." , 'a');
    key_t x = ftok("." , 'b');
    key_t y = ftok("." , 'c');
    key_t z = ftok("." , 'd');
    key_t v = ftok("." , 'e');

    a = shmget(w , 0 , IPC_CREAT | 0666);
    b = shmget(x , 0 , IPC_CREAT | 0666);
    c = shmget(y , 0 , IPC_CREAT | 0666);
    d = shmget(z , 0 , IPC_CREAT | 0666);
    e = shmget(v , 0 , IPC_CREAT | 0666);

    shmctl(a , IPC_RMID , 0);
    shmctl(b , IPC_RMID , 0);
    shmctl(c , IPC_RMID , 0);
    shmctl(d , IPC_RMID , 0);
    shmctl(e , IPC_RMID , 0);

    w = ftok("." , 'A');
    x = ftok("." , 'B');
    y = ftok("." , 'C');
    z = ftok("." , 'D');

    a = shmget(w , 0 , IPC_CREAT | 0666);
    b = shmget(x , 0 , IPC_CREAT | 0666);
    c = shmget(y , 0 , IPC_CREAT | 0666);
    d = shmget(z , 0 , IPC_CREAT | 0666);

    shmctl(a , IPC_RMID , 0);
    shmctl(b , IPC_RMID , 0);
    shmctl(c , IPC_RMID , 0);
    shmctl(d , IPC_RMID , 0);

    return 0;
}