#include "goodmalloc.h"
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

// ## Add print statements wherever appropriate

using namespace std;


class List{
    
public:
    string name;
    int scope;
    vector<char *> PageTable;
    int size;
    
    List(const string &_name="", int _size = 0);
    List(const List &);
    ~List();

};

int ERRNO = 0;
int curr_scope = 0;
char * buf = NULL;

set<char *> freePages;
map<pair<string,int> , List> Lists;


List::List(const string &_name , int _size):
name(_name) , size(_size) , scope(curr_scope)
{
    PageTable.clear();
}

List::List(const List & other):
name(other.name) , size(other.size) , scope(other.scope) ,  PageTable(other.PageTable)
{}

List::~List(){
    name.clear();
    PageTable.clear();
}

int scope_start(){
    curr_scope++;
    return 0;
}

int scope_end(){
    if(curr_scope - 1 < 0){
        ERRNO = SCOPE_ERR;
        exit(1);
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
    freePages.clear();

    return 0;
}


int getFreePages(int num_pages, vector<char *> &pages){
    if(freePages.size() == 0){
        ERRNO = SIZE_ERR;
        return -1;
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
        return -1;
    }
    pages.clear();
    for(int i=0;i<max_size;i++) pages.push_back(max_start + i*PAGE_SIZE);
    return 0;
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
    List l(name,size);
    l.scope = curr_scope;
    l.PageTable.clear();
    while(got_pages < num_pages){
        vector <char *> pages;
        if(getFreePages(num_pages - got_pages, pages) < 0){
            ERRNO = SIZE_ERR;
            return -1;
        }
        for(auto &it:pages){
            l.PageTable.push_back(it);
            freePages.erase(it);
        }
        got_pages += pages.size();
    }
    sort(l.PageTable.begin(),l.PageTable.end());
    Lists.insert(make_pair(make_pair(name,curr_scope),l));

    return 0;
}

int assignVal(const string &name , int offset , int val){
    auto curr_list = Lists.find({name,curr_scope});
    if(curr_list == Lists.end()){
        curr_list = Lists.find({name,0});
        if(curr_list == Lists.end()){
            ERRNO = LIST_NOT_FOUND_ERR;
            return -1;
        }
    }
    if(offset > curr_list->second.size - 4){
        ERRNO = SIZE_ERR;
        return -1;
    }

    int Q = offset / PAGE_SIZE;
    int R = offset % PAGE_SIZE;

    char *curr_page = curr_list->second.PageTable[Q];
    *((int *)(curr_page + R)) = val;
    
    return 0;
}

int readVal(const string &name , int offset , int * val){
    auto curr_list = Lists.find({name,curr_scope});
    if(curr_list == Lists.end()){
        curr_list = Lists.find({name,0});
        if(curr_list == Lists.end()){
            ERRNO = LIST_NOT_FOUND_ERR;
            return -1;
        }
    }
    if(offset > curr_list->second.size - 4){
        ERRNO = SIZE_ERR;
        return -1;
    }

    int Q = offset / PAGE_SIZE;
    int R = offset % PAGE_SIZE;

    char *curr_page = curr_list->second.PageTable[Q];
    *val = *((int *)(curr_page + R));

    return 0;
}

int freeList(const string &name){
    if(name == ""){
        for(auto curr_list = Lists.begin(); curr_list!=Lists.end(); ){
            if(curr_list->second.scope != curr_scope){
                curr_list++;
                continue;
            }
            for(auto &curr_page: curr_list->second.PageTable){
                freePages.insert(curr_page);
            }
            curr_list = Lists.erase(curr_list);
        }
        return 0;
    }
    
    auto curr_list = Lists.find({name , curr_scope});
    if(curr_list == Lists.end()){
        ERRNO = LIST_NOT_FOUND_ERR;
        return -1;
    }

    for(auto &curr_page: curr_list->second.PageTable){
        freePages.insert(curr_page);
    }
    Lists.erase(curr_list);

    return 0;
}
