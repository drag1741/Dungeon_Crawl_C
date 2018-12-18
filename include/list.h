/* File:list.h
   Author:Jay Drage
   
   Synapse:file to implement doubly linked lists in c
*/

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "node.h"

struct List{
    //members
    struct Node *head;
    struct Node *tail;
	int size;

    //Function pointers
    void (*add_node_end)(struct List*, struct Node*);
    void (*remove_node_mid)(struct List*, struct Node*);
    void (*delete_list)(struct List*);
    void (*print_list)(struct List*);
};

//initialize list
struct List* init_List(struct Node *node);

//add node to the end of list
void add_node_end_imp(struct List *list, struct Node *node);

//remove node from middle of list
void remove_node_mid_imp( struct List* head, struct Node *node);

//delete list
void delete_list_imp(struct List *list);

//print list from head to tail
void print_list_imp(struct List *head);

#endif
