/* File:list.c
   Author:Jay Drage
   
   Synapse:file to implement doubly linked lists in c
*/

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "node.h"

//initialize list
struct List* init_List(){
    struct List *ret_value;
    ret_value = malloc(sizeof(struct List));
    //set members
    ret_value->head = NULL;
	ret_value->tail = ret_value->head;
	ret_value->size = 1;
    //Function pointers
	ret_value->add_node_end = &add_node_end_imp;
	ret_value->remove_node_mid = &remove_node_mid_imp;
	ret_value->delete_list = &delete_list_imp;
	ret_value->print_list = &print_list_imp;
	//return
	return ret_value;
}
//add node to the end of list
void add_node_end_imp(struct List *list, struct Node *node){
    if(list->head == NULL){//list is empty
       list->head = node;
       list->tail = node; 
    }
    else{//list not empty
        node->prev = list->tail;
        node->next = NULL;
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}
//remove node from middle of list
void remove_node_mid_imp( struct List* list, struct Node *node){
    struct Node *current = list->head;
    struct Node *prev_node = current->prev;
    struct Node *next_node = current->next;
    while(current != node){
        if(current == NULL) return;//node not in list
        current = current->next;
    }
    if(current == list->head){//node is head of list
        list->head = current->next;    
        if(list->head != NULL) list->head->prev = NULL;
    }
    else if(current == list->tail){//node is tail of list
        prev_node = current->prev;
        prev_node->next = NULL;
        list->tail = prev_node;
    }
    else{//node is in mid of list
        prev_node = current->prev;
        next_node = current->next;
        prev_node->next = next_node;
        next_node->prev = prev_node;
    }
    list->size--;
    free(current);
}
//delete list
void delete_list_imp(struct List *list){
    struct Node *current = list->head;
    struct Node *prev_node = current;
    while(current != NULL){
        prev_node = current;
        free(prev_node);
        current = current->next;
    }
    free(list);
}
//print list from head to tail
void print_list_imp(struct List *list){
    if(list->head == NULL){
        fprintf(stderr,"list empty\n",list->size);
        return;
    }
    struct Node *current = list->head;
    fprintf(stderr,"list->size:%3d\n",list->size);
    while(current != NULL){
        current->print_node(current);
        current = current->next;
    }
}
