#ifndef GOOD_MALLOC_H
#define GOOD_MALLOC_H

#include <string>

#define PAGE_SIZE 4096      // ## Subject to change, currently 4KB
#define SCOPE_ERR 1
#define MEM_ERR 2
#define SIZE_ERR 3

extern int ERRNO;

class Page{

private:    
    char *startAddress;
    char *prev,*next;

public:
    Page(char * _startAddress);
    Page(const Page &);
    ~Page();

};

class List{
    
private:
    std::string name;
    int scope;
    char * startAddress;
    int size;

public:
    List(std::string _name , int _size);
    List(const List &);
    ~List();

};

int scope_start();

int scope_end();

int createMem(int size);

int createList(std::string name , int size);

int assignVal(std::string name , int offset , int val);

int freeList(std::string name = "");

#endif