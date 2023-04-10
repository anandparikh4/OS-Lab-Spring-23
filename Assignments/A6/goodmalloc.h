#ifndef GOOD_MALLOC_H
#define GOOD_MALLOC_H

#include <string>

#define PAGE_SIZE 4096      // ## Subject to change, currently 4KB
#define SCOPE_ERR 1
#define MEM_ERR 2
#define SIZE_ERR 3
#define DUPLICATE_ERR 4
#define UNKNOWN_ERR 5
#define NAME_ERR 6

extern int ERRNO;

int scope_start();

int scope_end();

int createMem(int size);

int createList(std::string name , int size);

int assignVal(std::string name , int offset , int val);

int readVal(std::string name , int offset , int * val);

int freeList(std::string name = "");

#endif