#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>

const int ARRAY_SIZE = 10;

int main() {
    // Create a unique key using ftok
    key_t key = ftok(".", 'S');

    // Access the shared memory segment using shmget
    int shm_id = shmget(key, ARRAY_SIZE * sizeof(int) * 2, 0666);

    // Attach the shared memory segment to the process using shmat
    int *shm_ptr = (int *)shmat(shm_id, nullptr, 0);

    // Print the two arrays in the shared memory segment
    std::cout << "Array 1: ";
    for (int i = 0; i < ARRAY_SIZE; i++) {
        std::cout << shm_ptr[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Array 2: ";
    for (int i =ARRAY_SIZE; i < ARRAY_SIZE * 2; i++) {
		std::cout << shm_ptr[i] << " ";
		}
		std::cout << std::endl;

		shmdt(shm_ptr);

return 0;
}
