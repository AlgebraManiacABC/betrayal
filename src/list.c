#include "../lib/list.h"

node make_node(void * obj, node next)
{
    node new = malloc(sizeof(node_s));
    if(!new)
    {
        fprintf(stderr,"Memory error! Could not create node!\n");
        return NULL;
    }
    new->obj = obj;
    new->next = next;
    return new;
}