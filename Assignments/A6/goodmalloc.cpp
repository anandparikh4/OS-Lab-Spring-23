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
    if(size <= 0){
        ERRNO = SIZE_ERR;
        return -1;
    }
    if(buf != NULL){
        ERRNO = MEM_ERR;
        return -1;
    }

    int Q = size / PAGE_SIZE;
    int R = size % PAGE_SIZE;
    int rounded_size = Q * PAGE_SIZE;
    if(R > 0) rounded_size += PAGE_SIZE;
    buf = (char *)malloc(rounded_size * sizeof(char));
    if(buf == NULL){
        ERRNO = SIZE_ERR;
        return -1;
    }

    Lists.clear();
    PageTable.clear();
    for(int i=0;i<rounded_size/PAGE_SIZE;i++) freePages.insert(buf + i*PAGE_SIZE);

    return 0;
}

int destroyMem(){
    if(buf == NULL){
        ERRNO = MEM_ERR;
        return -1;
    }

    free(buf);
    buf = NULL;
    Lists.clear();
    PageTable.clear();
    freePages.clear();

    return 0;
}


pair<char *,int> getFreePages(int num_pages){
    if(freePages.size() == 0){
        ERRNO = OVERFLOW_ERR;
        return make_pair((char *)NULL,-1);
    }
    auto it = freePages.begin();
    int curr_size = 0, max_size = 0;
    char * start = NULL, *max_start = NULL, *prev = NULL;
    while(it != freePages.end()){
        if(curr_size == 0) start = *it;
        if(prev != NULL && *it != prev + PAGE_SIZE){
            if(curr_size > max_size){
                max_size = curr_size;
                max_start = start;
            }
            curr_size = 0;
            start = NULL;
            prev = NULL;
            it++;
            continue;
        }
        curr_size++;
        prev = *it;
        it++;
        if(curr_size == num_pages){
            max_size = curr_size;
            max_start = start;
            break;
        }
    }
    if(max_size == 0){
        ERRNO = OVERFLOW_ERR;
        return make_pair((char *)NULL,-1);
    }
    return make_pair(max_start,max_size);

}


int createList(string &name , int size){
    if(name.size() == 0){
        ERRNO = NAME_ERR;
        return -1;
    }
    if(size <= 0){
        ERRNO = SIZE_ERR;
        return -1;
    }
    auto it = Lists.find({name,curr_scope});
    if(it != Lists.end()){
        ERRNO = DUPLICATE_ERR;
        return -1;
    }

    int Q = size / PAGE_SIZE;
    int R = size % PAGE_SIZE;
    int num_pages = Q;
    if(R > 0) num_pages++;

    if(num_pages > freePages.size()){
        ERRNO = OVERFLOW_ERR;
        return -1;
    }
    int got_pages = 0;
    vector<char *> start_pages;
    while(got_pages < num_pages){
        auto p = getFreePages(num_pages - got_pages);
        if(p.second == -1){
            ERRNO = OVERFLOW_ERR;
            return -1;
        }
        got_pages += p.second;
        start_pages.push_back(p.first);
    }

    List l(name,size);

}




/*

int createList(std::string name , int size);

int assignVal(std::string name , int offset , int val);

int freeList(std::string name = "");

#endif
*/