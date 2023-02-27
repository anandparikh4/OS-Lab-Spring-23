#include <iostream>
#include <unistd.h>
#include <time.h>
#include <vector>
#include "graph.h"
#include <fstream>

using namespace std;

int main(){
    // sleep(10000);
    srand(time(NULL));

    graph * G = activate_graph();
    
    while(1){
        int n = *(G->n);        // current number of nodes in the graph
        int sum = 0;
        vector<int> pf(n+1);
        pf[0] = 0;
        for(int i=0;i<n;i++){
            int degree = (G->heads[i]).degree;
            sum += degree;
            pf[i+1] = pf[i] + degree;        // maintain prefix sums of degree to binary search on which node to connect with
        }

        int m = rand()%21 + 10;             // m ~ U[10,30]
        
        for(int i=0;i<m;i++){
            G = add_node(G , n+i);
            int k = rand()%20 + 1;          // k ~ U[1,20]
            for(int j=0;j<k;j++){
                int x = rand()%sum;
                int l = 0;
                int r = n;
                int ans = -1;
                while(l <= r){
                    int mid = l+(r-l)/2;
                    if(x < pf[mid]) r = mid-1;
                    else{
                        ans = mid;
                        l = mid+1;
                    }
                }
                // ans == -1 will never occur, nor will ans = n, since the rand function gives value in [0,sum-1] 
                // also, it automatically gives the LAST number which is <= x, so degrees that are 0 do not pose a problem
                // cout << n+i << " " << ans << endl;
                G = add_edge(G , n+i , ans);
            }
        }

    // // uncomment following code for testing whether the shared memory and graph data structure implementation work correctly
    // // TESTING
    //     ofstream F;
    //     F.open("test2.txt");        // graph from shared memory
    //     vector<vector<int>> b(4039 , vector<int>(0));
    //     for(int i=0;i<*(G->n);i++){
    //         F << i << ": ";
    //         int curr = (G->heads[i]).next;
    //         while(curr != -1){
    //             F << G->nodes[curr].id << " ";
    //             curr = G->nodes[curr].next;
    //         }
    //         F << endl;
    //     }
    //     F.close();
    // // TESTING
        sleep(50);  // ## change to 50
    }

    deactivate_graph(G);

    return 0;
}