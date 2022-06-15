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

list init_list()
{
    return make_node(NULL,NULL);
}

int insert_node(list head, void * obj)
{
    node temp = head;
    while(temp && temp->next)
        temp = temp->next;
    temp->next = make_node(obj,NULL);
    if(!temp->next)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void * remove_head(list head)
{
    node pop = head->next;
    head->next = pop->next;
    void * obj = pop->obj;
    free(pop);
    return obj;
}

void * remove_next(node here)
{
    node next = here->next;
    void * obj = next->obj;
    here->next = next->next;
    free(next);
    return obj;
}

void free_list(list head)
{
    while(head->next)
    {
        node temp = head->next;
        head->next = temp->next;
        free(temp);
    }
    free(head);
}

bool empty(list head)
{
    if(head->next)
        return false;
    return true;
}

int list_length(list head)
{
    node temp = head;
    int count = 0;

    while(temp && (temp = temp->next) )
        count++;

    return count;
}