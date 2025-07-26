#include "goodmalloc.h"
#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

// Merge two subarrays of arr[]
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(string &arr, int l, int m, int r){
    scope_start();
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    string L = "L";
    string R = "R";
    if(createList(L, 4*n1) < 0){
        cout << "Error in creating list L" << endl;
        cout<<"Error no: "<<ERRNO<<endl;
        exit(1);
    }
    if(createList(R, 4*n2) < 0){
        cout << "Error in creating list R" << endl;
        cout<<"Error no: "<<ERRNO<<endl;
        exit(1);
    }
    int val,val2;
    // Copy data to temp arrays L[] and R[]
    for(i = 0; i < n1; i++){
        if(readVal(arr, 4*(l+i), &val)<0){
            cout<<"Error in read arr"<<endl;
            cout<<"Error no: "<<ERRNO<<endl;
            exit(1);
        }
        if(assignVal(L, 4*i, val)<0){
            cout<<"Error in assign L"<<endl;
            cout<<"Error no: "<<ERRNO<<endl;
            exit(1);
        }
        // L[i] = arr[l + i];
    }
    for(j = 0; j < n2; j++){
        if(readVal(arr, 4*(m+1+j), &val)<0){
            cout<<"Error in read arr"<<endl;
            cout<<"Error no: "<<ERRNO<<endl;
            exit(1);
        }
        if(assignVal(R, 4*j, val)<0){
            cout<<"Error in assign R"<<endl;
            cout<<"Error no: "<<ERRNO<<endl;
            exit(1);
        }
        // R[j] = arr[m + 1 + j];
    }
    i=0;
    j=0;
    k=l;
    while(i < n1 && j < n2){
        if(readVal(L, 4*i, &val)<0){
            cout<<"Error in read L"<<endl;
            cout<<"Error no: "<<ERRNO<<endl;
            exit(1);
        }
        if(readVal(R, 4*j, &val2)<0){
            cout<<"Error in read R"<<endl;
            cout<<"Error no: "<<ERRNO<<endl;
            exit(1);
        }
        if(val <= val2){
            if(assignVal(arr, 4*k, val)<0){
                cout<<"Error in assign arr"<<endl;
                cout<<"Error no: "<<ERRNO<<endl;
                exit(1);
            }
            i++;
        }
        else{
            if(assignVal(arr, 4*k, val2)<0){
                cout<<"Error in assign arr"<<endl;
                cout<<"Error no: "<<ERRNO<<endl;
                exit(1);
            }
            j++;
        }
        k++;
        // if(L[i] <= R[j]){
        //     arr[k++] = L[i++];
        // }
        // else{
        //     arr[k++] = R[j++];
        // }
    }

    // Copy the remaining elements of L[], if there are any
    while(i < n1){
        if(readVal(L, 4*i, &val)<0){
            cout<<"Error in read L"<<endl;
            cout<<"Error no: "<<ERRNO<<endl;
            exit(1);
        }
        if(assignVal(arr, 4*k, val)<0){
            cout<<"Error in assign arr"<<endl;
            cout<<"Error no: "<<ERRNO<<endl;
            exit(1);
        }
        // arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while(j < n2){
        if(readVal(R, 4*j, &val)<0){
            cout<<"Error in read R"<<endl;
            cout<<"Error no: "<<ERRNO<<endl;
            exit(1);
        }
        if(assignVal(arr, 4*k, val)<0){
            cout<<"Error in assign arr"<<endl;
            cout<<"Error no: "<<ERRNO<<endl;
            exit(1);
        }
        // arr[k] = R[j];
        j++;
        k++;
    }

    freeList();
    scope_end();
}


// Recursive Marge Sort
void mergeSort(string &arr, int l, int r){
    if(l < r){
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}




int main(){

    srand(time(NULL));
    struct timespec start, end;
    ofstream myfile;
    myfile.open("mergeSort_output.txt", ios::out);
    clock_gettime(CLOCK_MONOTONIC, &start);
    if(createMem(250000000) < 0){
        printf("Error: %d\n" , ERRNO);
        exit(0);
    }
    // vector<int> v;
    int val;
    string arr = "arr";
    if(createList(arr, 200000) < 0){
        printf("Error: %d\n" , ERRNO);
        exit(0);
    }
    for(int i = 0; i < 50000; i++){
        val = rand()%100000 + 1;
        // v.push_back(val);
        if(assignVal(arr, 4*i, val)<0){
            cout << "Error in assigning value: ";
            cout << ERRNO << endl;
            exit(0);
        }
    }

    myfile<<"Before sorting: \n";
    for(int i = 0; i < 50000; i++){
        int val;
        if(readVal(arr, 4*i, &val)<0){
            cout << "Error in reading value: ";
            cout << ERRNO << endl;
            exit(0);
        }
        myfile << val << " ";
        if(i%20 == 19){
            myfile << "\n";
        }
    }
    myfile << "\n\n"<<endl;

    mergeSort(arr, 0, 49999);

    // sort(v.begin(), v.end());

    myfile<<"After sorting: \n";
    for(int i = 0; i < 50000; i++){
        int val;
        if(readVal(arr, 4*i, &val)<0){
            cout << "Error in reading value: ";
            cout << ERRNO << endl;
            exit(0);
        }
        myfile << val << " ";
        // if(val != v[i]){
        //     cout << "Error in sorting" << endl;
        //     exit(0);
        // }
        if(i%20 == 19){
            myfile << "\n";
        }
    }
    myfile << "\n\n";
    freeList(arr);
    destroyMem();
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e3;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)/1000000.0);
    myfile << "Time taken by program is : " << time_taken;
    myfile << " milli sec " << endl;
    return 0;
}