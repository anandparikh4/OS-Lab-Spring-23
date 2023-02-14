#include <sys/ipc.h>
#include <sys/shm.h>
#include <bits/stdc++.h>

using namespace std;

int main() {

    vector<int> left,right;
     ifstream File;
    File.open("graph.txt");
    int left_ele, right_ele;
    while (File >> left_ele >> right_ele){      
        left.push_back(left_ele);
        right.push_back(right_ele);
    }
    File.close();

    int num_ele = left.size();

     key_t key_3 = ftok(".", 'c');

    // Create a shared memory segment using shmget
    int shm_id_3 = shmget(key_3, 2 * sizeof(int), IPC_CREAT | 0666);
    
    // Create a unique key using ftok
    key_t key_1 = ftok(".", 'a');

    // Create a shared memory segment using shmget
    int shm_id_1 = shmget(key_1, num_ele * sizeof(int), IPC_CREAT | 0666);

    key_t key_2 = ftok(".", 'b');

    // Create a shared memory segment using shmget
    int shm_id_2 = shmget(key_2, num_ele * sizeof(int) * 2, IPC_CREAT | 0666);

    


    // Attach the shared memory segment to the process using shmat
    int *left_s = (int *)shmat(shm_id_1, nullptr, 0);
    int *right_s = (int *)shmat(shm_id_2, nullptr, 0);
    int *dim = (int *)shmat(shm_id_3, nullptr, 0);

    // left and right are vectors that store the edges: (left[i] , right[i]) is an edge
    // uncomment last 4 lines to see the numbers in each vector and the number of elements (same in both vectors)

    dim[0] = max(*max_element(left.begin(), left.end()),*max_element(right.begin(), right.end()))+1;
    dim[1] = left.size();

   
    for(int i=0;i<num_ele;i++){
        left_s[i] = left[i];
        right_s[i] = right[i];
        // cout << left[i] << " " <<  right[i] << endl;
    }
    // cout << "Sizes " << left.size() << " " << right.size() << endl;
    

    
    // Detach the shared memory segment from the process
    shmdt(left_s);
    shmdt(right_s);
    shmdt(dim);
    return 0;
}
