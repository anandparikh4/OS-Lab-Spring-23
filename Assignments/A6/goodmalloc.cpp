#include "goodmalloc.h"
#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <map>

// ## Add print statements wherever appropriate

using namespace std;

class Page{

public:    
    char *startAddress;
    char *prev,*next;

    Page(char * _startAddress = NULL);
    Page(const Page &);
    ~Page();

};

class List{
    
public:
    string name;
    int scope;
    char * startAddress;
    int size;
    
    List(const string &_name="", int _size = 0);
    List(const List &);
    ~List();

};

int ERRNO = 0;
int curr_scope = 0;
char * buf = NULL;

set<char *> freePages;
map<char *,Page> PageTable;
map<pair<string,int> , List> Lists;

Page::Page(char * _startAddress):
startAddress(_startAddress)
{
    prev = next = NULL;
}

Page::Page(const Page & other):
startAddress(other.startAddress) , prev(other.prev) , next(other.next)
{}

Page::~Page(){
    startAddress = NULL;
    prev = next = NULL;
}

List::List(const string &_name , int _size):
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
        ERRNO = SIZE_ERR;
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
        ERRNO = SIZE_ERR;
        return make_pair((char *)NULL,-1);
    }
    return make_pair(max_start,max_size);

}


int createList(const string &name , int size){
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
        ERRNO = SIZE_ERR;
        return -1;
    }
    int got_pages = 0;
    char *prev=NULL,*start_page=NULL;
    while(got_pages < num_pages){
        auto p = getFreePages(num_pages - got_pages);
        if(p.second == -1){
            ERRNO = SIZE_ERR;
            return -1;
        }
        if(start_page == NULL) start_page = p.first;
        for(int i=0;i<p.second;i++){
            PageTable.insert({{p.first + i*PAGE_SIZE},Page(p.first + i*PAGE_SIZE)});
            if(prev != NULL){
                PageTable[prev].next = p.first + i*PAGE_SIZE;
                PageTable[p.first + i*PAGE_SIZE].prev = prev;
            } 
            prev = p.first + i*PAGE_SIZE;
            freePages.erase(p.first + i*PAGE_SIZE);
        }
        got_pages += p.second;
    }
    PageTable[prev].next = NULL;
    List l(name,size);
    l.startAddress = start_page;
    l.scope = curr_scope;
    Lists.insert({{name,curr_scope},l});

    return 0;
}

int assignVal(const string &name , int offset , int val){
    auto curr_list = Lists.find({name,curr_scope});
    if(curr_list == Lists.end()){
        ERRNO = UNKNOWN_ERR;
        return -1;
    }
    if(offset > curr_list->second.size - 4){
        ERRNO = SIZE_ERR;
        return -1;
    }

    int Q = offset / PAGE_SIZE;
    int R = offset % PAGE_SIZE;

    Page * curr_page = &(PageTable[curr_list->second.startAddress]);
    while(Q--) curr_page = &(PageTable[curr_page->next]);
    *(int *)(curr_page->startAddress + R) = val;
    
    return 0;
}

int readVal(const string &name , int offset , int * val){
    auto curr_list = Lists.find({name,curr_scope});
    if(curr_list == Lists.end()){
        ERRNO = UNKNOWN_ERR;
        return -1;
    }
    if(offset > curr_list->second.size - 4){
        ERRNO = SIZE_ERR;
        return -1;
    }

    int Q = offset / PAGE_SIZE;
    int R = offset % PAGE_SIZE;

    Page * curr_page = &(PageTable[curr_list->second.startAddress]);
    while(Q--) curr_page = &(PageTable[curr_page->next]);
    *val = *(int *)(curr_page->startAddress + R);

    return 0;
}

int freeList(const string &name){
    if(name == ""){
        vector<string> delete_lists(0);
        for(auto &curr_list : Lists){
            if(curr_list.second.scope != curr_scope) continue;
            delete_lists.push_back(curr_list.second.name);
            Page curr_page = PageTable[curr_list.second.startAddress];
            while(1){
                PageTable.erase(curr_page.startAddress);
                freePages.insert(curr_page.startAddress);
                if(curr_page.next == NULL) break;
                curr_page = PageTable[curr_page.next];
            }
        }
        for(auto &curr_list : delete_lists){
            Lists.erase(Lists.find({curr_list , curr_scope}));
        }
        return 0;
    }
    
    auto it = Lists.find({name , curr_scope});
    if(it == Lists.end()){
        ERRNO = UNKNOWN_ERR;
        return -1;
    }

    Page curr_page = PageTable[it->second.startAddress];
    while(1){
        PageTable.erase(curr_page.startAddress);
        freePages.insert(curr_page.startAddress);
        if(curr_page.next == NULL) break;
        curr_page = PageTable[curr_page.next];
    }
    Lists.erase(it);

    return 0;
}
