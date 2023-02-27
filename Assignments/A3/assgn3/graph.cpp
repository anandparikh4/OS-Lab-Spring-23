#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>
#include "graph.h"

using namespace std;

int swap_id;
int * swap_val;

// 0 nodes, 0 edges
void clear_graph(graph * G){
    *(G->n) = 0;
    *(G->top) = 0;
}

void set_swap(int s){
    key_t swap_key = ftok("." , 'e');
    swap_id = shmget(swap_key , sizeof(int) , IPC_CREAT | 0666);
    swap_val = (int *)shmat(swap_id , 0 , 0);
    if(s!=-1) *swap_val = s;
    return;
}

// attach (optionally create) calling process to this graph in shared memory
graph* activate_graph(int create_flag , int heads_max , int nodes_max){

    if(swap_val == 0) set_swap();

    graph * G = (graph *)malloc(sizeof(graph));

    // get unique keys
    key_t heads_key = ftok("." , 'a');
    key_t nodes_key = ftok("." , 'b');
    key_t top_key = ftok("." , 'c');
    key_t n_key = ftok("." , 'd');

    if(*swap_val){
        heads_key = ftok("." , 'A');
        nodes_key = ftok("." , 'B');
        top_key = ftok("." , 'C');
        n_key = ftok("." , 'D');
    }

    // if create_flag is set, then use inital sizes, otherwise use 0
    int heads_size = 0, nodes_size = 0, top_size = 0, n_size = 0;
    if(create_flag){
        heads_size = G->heads_MAX = heads_max;
        nodes_size = G->nodes_MAX = nodes_max;
        top_size = 1;
        n_size = 1;
    }

    // create / get shared memory segments
    if((G->heads_id = shmget(heads_key , heads_size * sizeof(struct head) , IPC_CREAT | 0666)) < 0){
        perror("shmget");
        exit(0);
    }
    if((G->nodes_id = shmget(nodes_key , nodes_size * sizeof(struct node) , IPC_CREAT | 0666)) < 0){
        perror("shmget");
        exit(0);
    }
    if((G->top_id = shmget(top_key , top_size * sizeof(int) , IPC_CREAT | 0666)) < 0){
        perror("shmget");
        exit(0);
    }
    if((G->n_id = shmget(n_key , n_size * sizeof(int) , IPC_CREAT | 0666)) < 0){
        perror("shmget");
        exit(0);
    }

    // attach shared memory segments
    void * ret_val = NULL;

    ret_val = shmat(G->heads_id , 0 , 0);
    if(ret_val == (void *)(-1)){
        perror("shmat");
        exit(0);
    }
    else G->heads = (head *)ret_val;

    ret_val = shmat(G->nodes_id , 0 , 0);
    if(ret_val == (void *)(-1)){
        perror("shmat");
        exit(0);
    }
    else G->nodes = (node *)ret_val;

    ret_val = shmat(G->top_id , 0 , 0);
    if(ret_val == (void *)(-1)){
        perror("shmat");
        exit(0);
    }
    else G->top = (int *)ret_val;
    
    ret_val = shmat(G->n_id , 0 , 0);
    if(ret_val == (void *)(-1)){
        perror("shmat");
        exit(0);
    }
    else G->n = (int *)ret_val;

    if(create_flag) clear_graph(G);

    return G;
}

// detach (optionally deallocate) calling process from the graph in shared memory
void deactivate_graph(graph * G , int destroy_flag){
    // detach shared memory segments
    if(shmdt(G->heads) < 0){
        perror("shmdt");
        exit(0);
    }
    if(shmdt(G->nodes) < 0){
        perror("shmdt");
        exit(0);
    }
    if(shmdt(G->top) < 0){
        perror("shmdt");
        exit(0);
    }
    if(shmdt(G->n) < 0){
        perror("shmdt");
        exit(0);
    }

    // free shared memory segments
    if(destroy_flag){
        if(shmctl(G->heads_id , IPC_RMID , 0) < 0){
            perror("shmctl");
            exit(0);
        }
        if(shmctl(G->nodes_id , IPC_RMID , 0) < 0){
            perror("shmctl");
            exit(0);
        }
        if(shmctl(G->top_id , IPC_RMID , 0) < 0){
            perror("shmctl");
            exit(0);
        }
        if(shmctl(G->n_id , IPC_RMID , 0) < 0){
            perror("shmctl");
            exit(0);
        }
        if(destroy_flag == 2){          // remove this only when destroyed by main, not otherwise
            if(shmdt(swap_val) < 0){
                perror("shmdt");
                exit(0);
            }
            if(shmctl(swap_id , IPC_RMID , 0) < 0){
                perror("this one ? shmctl");
                exit(0);
            }
        }
    }

    free(G);

    return;
}

graph* reallocate_graph(graph * G , int cause){
    *swap_val = 1 - *swap_val;
    graph * temp;      // make new temporary graph object

    int new_heads_MAX = G->heads_MAX;
    int new_nodes_MAX = G->nodes_MAX;
    if(cause == 0) new_heads_MAX *= 2;
    if(cause == 1) new_nodes_MAX *= 2;

    temp = activate_graph(1 , new_heads_MAX , new_nodes_MAX);    // create and attach a new graph with twice the capacity

    // copy all data from old to new
    for(int i=0;i<*(G->n);i++){
        temp->heads[i] = G->heads[i];
    }
    *(temp->n) = *(G->n);
    for(int i=0;i<*(G->top);i++){
        temp->nodes[i] = G->nodes[i];
    }
    *(temp->top) = *(G->top);

    deactivate_graph(G , 1);   // detach and destroy old shared memory

    return temp;
}

// add a node to the shared memory graph
graph* add_node(graph * G , int id){
    if(*(G->n) == G->heads_MAX){
        G = reallocate_graph(G , 0);
    }
    (G->heads[id]).id = id;
    (G->heads[id]).degree = 0;
    (G->heads[id]).next = -1;
    (*(G->n))++;
    return G;
}

// add an edge to the shared memory graph
graph* add_edge(graph * G , int lid , int rid , int directed){

    if(*(G->top) == G->nodes_MAX){
        G = reallocate_graph(G , 1);
    }

    // add rid to front of list of lid
    (G->nodes[*(G->top)]).id = rid;
    (G->nodes[*(G->top)]).next = (G->heads[lid]).next;
    (G->heads[lid]).next = *(G->top);
    (G->heads[lid]).degree++;
    (*(G->top))++;
    
    if(directed) return G;    // if it is directed, then do not add an edge in the opposite direction

    // add lid to front of list of rid
    (G->nodes[*(G->top)]).id = lid;
    (G->nodes[*(G->top)]).next = (G->heads[rid]).next;
    (G->heads[rid]).next = *(G->top);
    ((G->heads[rid]).degree)++;
    (*(G->top))++;
    return G;
}
