#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
using namespace std;



int main() {
    // Create a unique key using ftok
     
    key_t key_3 = ftok(".", 'c');

    // Create a shared memory segment using shmget
    int shm_id_3 = shmget(key_3, 2 * sizeof(int), IPC_CREAT | 0666);

    int *dim = (int *)shmat(shm_id_3, nullptr, 0);

    int num_ele = dim[1] ;

    key_t key_1 = ftok(".", 'a');

    // Create a shared memory segment using shmget
    int shm_id_1 = shmget(key_1, num_ele * sizeof(int), IPC_CREAT | 0666);

    key_t key_2 = ftok(".", 'b');

    // Create a shared memory segment using shmget
    int shm_id_2 = shmget(key_2, num_ele * sizeof(int) * 2, IPC_CREAT | 0666);

    // Attach the shared memory segment to the process using shmat
    int *left_s = (int *)shmat(shm_id_1, nullptr, 0);
    int *right_s = (int *)shmat(shm_id_2, nullptr, 0);

  

    std::cout << "Array 2: ";
    for (int i =0; i < num_ele; i++) {
		cout << left_s[i] << " " <<  right_s[i] << endl;
    }
     cout << "Sizes " << dim[1] << " " << dim[0] << endl;

    
    shmdt(left_s);
    shmdt(right_s);
    shmdt(dim);


return 0;
}
