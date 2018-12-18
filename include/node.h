/* File:node.h
*  Author:Jay Drage
*
*  Synapse:node struct for data structures 
*/

#ifndef NODE_H
#define NODE_H

enum Type{  character = 0, 
		    item = 1,
			monster = 2,
			floor = 3,
			integer = 4
};

struct Node{
	//members
    void *prev;
    void *next;
    void *data;
	int index;
	enum Type type;
	
	//functions
    void (*print_node)(struct Node*);
};

//init a new Node with void *data == data
struct Node* init_Node(void* data, enum Type type);
//print out data from node
void print_node_imp(struct Node *node);
#endif
