#include "goodmalloc.h"
#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <map>

// ## Add print statements wherever appropriate

using namespace std;

int ERRNO = 0;
int curr_scope = 0;
char * buf = NULL;

set<char *> freePages;
map<char *,Page> PageTable;
map<pair<string,int> , List> Lists;

Page::Page(char * _startAddress):
startAddress(_startAddress)
{}

Page::Page(const Page & other):
startAddress(other.startAddress) , prev(other.prev) , next(other.next)
{}

Page::~Page(){
    startAddress = NULL;
    prev = next = NULL;
}

List::List(std::string _name , int _size):
name(_name) , size(_size) , scope(curr_scope) , startAddress(NULL)
{}

List::List(const List & other):
name(other.name) , size(other.size) , scope(other.scope) ,  startAddress(other.startAddress)
{}

List::~List(){
    startAddress = NULL;
}

int scope_start(){
    curr_scope++;
    return 0;
}

int scope_end(){
    if(curr_scope - 1 < 0){
        ERRNO = SCOPE_ERR;
        return -1;
    }
    curr_scope--;
    return 0;
}

int createMem(int size){
    if(buf != NULL){
        ERRNO = MEM_ERR;
        return -1;
    }
    buf = (char *)malloc(size * sizeof(char));
    if(buf == NULL){
        ERRNO = SIZE_ERR;
        return -1;
    }
    return 0;
}

int destroyMem(){
    if(buf == NULL){
        ERRNO = MEM_ERR;
        return -1;
    }
    free(buf);
    buf = NULL;
    return 0;
}



/*

int createList(std::string name , int size);

int assignVal(std::string name , int offset , int val);

int freeList(std::string name = "");

#endif
*/