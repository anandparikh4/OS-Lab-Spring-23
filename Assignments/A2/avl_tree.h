#ifndef _AVL_TREE_H
#define _AVL_TREE_H

struct Node {
  int key;
  struct Node *left;
  struct Node *right;
  int height;
};

struct Node *newNode(int key);
struct Node *insertNode(struct Node *node, int key);
struct Node *findNode(struct Node * root , int x);
struct Node *deleteNode(struct Node *root, int key);
void destroyTree(struct Node *root);

#endif

// provide user access only to newNode() , insertNode() , findNode() , deleteNode() , destroyTree()