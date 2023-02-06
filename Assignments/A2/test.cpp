#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;


int main()
{
    FILE *fp;
    fp = fopen("t3.txt", "w");
    while(1){
        // fprintf(fp, "Hello World\n");
        sleep(10);
    }
    return 0;
}