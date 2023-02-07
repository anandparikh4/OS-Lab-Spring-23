#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <libgen.h>
using namespace std;


int main()
{
    // FILE *fp;
    // fp = fopen("t3.txt", "w");
    // while(1){
    //     // fprintf(fp, "Hello World\n");
    //     sleep(10);
    // }
    char t[100];
    cin>>t;
    char *dir_path = strdup(t);
    dir_path = dirname(dir_path);
    cout<<dir_path<<'\n';
    return 0;
}