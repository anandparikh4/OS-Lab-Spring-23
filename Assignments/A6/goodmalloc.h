#ifndef GOOD_MALLOC_H
#define GOOD_MALLOC_H

#include <string>

#define PAGE_SIZE 256      // ## Subject to change, currently 4KB
#define SCOPE_ERR 1
#define MEM_ERR 2
#define SIZE_ERR 3
#define DUPLICATE_ERR 4
#define LIST_NOT_FOUND_ERR 5
#define NAME_ERR 6

extern int ERRNO;

int scope_start();

int scope_end();

int createMem(int size);

int createList(const std::string &name , int size);

int assignVal(const std::string &name , int offset , int val);

int readVal(const std::string &name , int offset , int * val);

int freeList(const std::string &name = "");

int destroyMem();

#endif