#include "goodmalloc.h"
#include <iostream>

using namespace std;

// Merge two subarrays of arr[]
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int *arr, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    int L[n1], R[n2];

    // Copy data to temp arrays L[] and R[]
    for(i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for(j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i=0;
    j=0;
    k=l;
    while(i < n1 && j < n2){
        if(L[i] <= R[j]){
            arr[k++] = L[i++];
        }
        else{
            arr[k++] = R[j++];
        }
    }

    // Copy the remaining elements of L[], if there are any
    while(i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while(j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}


// Recursive Marge Sort
void mergeSort(int *arr, int l, int r){
    if(l < r){
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}




int main(){
    createMem(10000);
    scope_start();
    cout << "Hello World!" << endl;
    for(int i = 0; i < 10; i++){
        if(createList((string("list")+to_string(i)), 1000)<0){
            cout << "Error creating list" << endl;
            cout << "Error code: " << ERRNO << endl;
        }
        else{
            cout <<i<< "th List created" << endl;
        }
    }
    scope_end();
    destroyMem();
    return 0;
}