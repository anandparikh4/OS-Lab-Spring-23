#ifndef _GRAPH_H
#define _GRAPH_H

struct node{                    // structure of nodes in the so-called "adjacency list"
    int id;
    int next = -1;
};

typedef struct node node;

struct head{                    // structure of head nodes in the so-called "adjacency list"
    int id;
    int next = -1;
    int degree = 0;
};

typedef struct head head;

struct graph{
    int heads_id;
    int nodes_id;
    int top_id;
    int n_id;
    int heads_MAX;
    int nodes_MAX;

    head * heads;
    node * nodes;
    int * top;
    int * n;
};

typedef struct graph graph;

graph* activate_graph(int create_flag = 0 , int heads_max = 5000 , int nodes_max = 250000);

void deactivate_graph(graph * , int destroy_flag = 0);

graph* add_node(graph * , int id);

graph* add_edge(graph * , int l , int r , int directed = 0);

void set_swap(int s = -1);

#endif
