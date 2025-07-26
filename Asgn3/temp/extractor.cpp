#include<bits/stdc++.h>

using namespace std;

int main(){
    // left and right are vectors that store the edges: (left[i] , right[i]) is an edge
    // uncomment last 4 lines to see the numbers in each vector and the number of elements (same in both vectors)
    vector<int> left,right;

    ifstream File;
    File.open("graph.txt");
    int num,side = 0;
    while (File >> num){      
        if(side == 0){
            left.push_back(num);
            side = 1;
        }
        else{
            right.push_back(num);
            side = 0;
        }
    }
    File.close();

    // for(int i=0;i<left.size();i++){
    //     cout << left[i] << " " <<  right[i] << endl;
    // }
    // cout << "Sizes " << left.size() << " " << right.size() << endl;

return 0;
}