#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>

const int ARRAY_SIZE = 10;

int main() {
    // Create a unique key using ftok
    key_t key = ftok(".", 'S');

    // Create a shared memory segment using shmget
    int shm_id = shmget(key, ARRAY_SIZE * sizeof(int) * 2, IPC_CREAT | 0666);

    // Attach the shared memory segment to the process using shmat
    int *shm_ptr = (int *)shmat(shm_id, nullptr, 0);

    // Initialize the two arrays in the shared memory segment
    int array1[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int array2[ARRAY_SIZE] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    for (int i = 0; i < ARRAY_SIZE; i++) {
        shm_ptr[i] = array1[i];
        shm_ptr[i + ARRAY_SIZE] = array2[i];
    }

    // Detach the shared memory segment from the process
    shmdt(shm_ptr);

    return 0;
}
