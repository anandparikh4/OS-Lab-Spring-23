#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;


int main()
{
    int fd;
    struct flock lock;

    // Open the file for read and write
    fd = open("t3.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    // printf("Heelo\n");
    // Initialize the lock structure
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    // Acquire the lock
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    // printf("Heelo\n");
    // FILE *fp;
    // fp = fopen("t3.txt", "w");
    int t=4;
    while(t--){
        // fprintf(fp, "Hello World\n");
        printf("%d\n",t);
        sleep(8);
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // Close the file
    close(fd);
    return 0;
}