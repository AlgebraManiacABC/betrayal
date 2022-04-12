#include "main.h"

typedef struct node * node;

typedef node list;

typedef struct node
{
    void * obj;
    node next;

}   node_s;

node make_node(void *obj, node next);