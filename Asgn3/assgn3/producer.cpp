#include <iostream>
#include <unistd.h>
#include <time.h>
#include <vector>
#include "graph.h"

using namespace std;

int main(){
    
    srand(time(NULL));

    graph * G = activate_graph();   // connect to shared memory graph
    
    while(1){
        sleep(50);

        int n = *(G->n);        // current number of nodes in the graph
        int sum = 0;
        vector<int> pf(n+1);
        pf[0] = 0;
        for(int i=0;i<n;i++){
            int degree = (G->heads[i]).degree;
            sum += degree;
            pf[i+1] = pf[i] + degree;       // maintain prefix sums of degree to binary search on which node to connect with
        }

        int m = rand()%21 + 10;             // m ~ U[10,30] : New nodes to be added
        
        for(int i=0;i<m;i++){
            G = add_node(G , n+i);
            int k = rand()%20 + 1;          // k ~ U[1,20] :  New edges to be added for each newly added node
            for(int j=0;j<k;j++){
                int x = rand()%sum;         // x ~ U[0,sum-1] : Decides which node to connect to
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
                // ans = -1 will never occur, nor will ans = n, since pf[0] = 0 <= min(x) = 0 and pf[n] = sum > max(x) = sum-1 
                // also, it automatically gives the LAST number which is <= x, so degrees that are 0 do not pose a problem (even though '0' degrees are absent)
                // cout << n+i << " " << ans << endl;
                G = add_edge(G , n+i , ans);
            }
        }
    }

    deactivate_graph(G);    // disconnect from shared memory graph

    return 0;
}