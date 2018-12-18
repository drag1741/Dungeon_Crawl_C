/* File:node.c
   Author: Jay Drage

   Synapse:node struct for data structures
*/

#include <stdio.h>
#include <stdlib.h>

#include "node.h"

//init a new Node with void *data == data
struct Node* init_Node(void *data, enum Type type){
	//allocate memory
	struct Node *ret_value = NULL;
	ret_value = malloc(sizeof(struct Node));
	//set data
	ret_value->prev = NULL;
	ret_value->next = NULL;
	ret_value->data = data;
	ret_value->index = 0;
	ret_value->type = type;
    //set function pointers
    ret_value->print_node = &print_node_imp;
	//return
	return ret_value;
}

//print out data from node
void print_node_imp(struct Node *node){
    switch(node->type){
        case character:
            fprintf(stdout,"character(%d):%3c\n",node->type,*(int*)node->data);
            break;
        case item:
            fprintf(stdout,"item(%d):%3c\n",node->type,*(int*)node->data);
            break;
        case monster:
            fprintf(stdout,"monster(%d):%3c\n",node->type,*(int*)node->data);
            break;
        case floor:
            break;
        case integer:
            fprintf(stdout,"integer(%d):%3d\n",node->type,*(int*)node->data);
            break;
    }
}
