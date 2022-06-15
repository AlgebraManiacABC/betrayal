#include "main.h"

typedef struct node * node;

typedef node list;

typedef struct node
{
    void * obj;
    node next;

}   node_s;

node make_node(void *obj, node next);

list init_list();

int insert_node(list head, void *obj);

void * remove_head(list head);

void * remove_next(node here);

void free_list(list head);

bool empty(list head);

int list_length(list head);