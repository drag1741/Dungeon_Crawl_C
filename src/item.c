//item.c
//implementation of item struct

#include <stdio.h>
#include <stdlib.h>
#include "../include/item.h"

//initialize the item 
struct Item* init_Item(const char symbol, const int y, const int x){
    //allocate memory
    struct Item *ret_value = malloc(sizeof (struct Item));
    if (ret_value == NULL){ //malloc failed
		fprintf(stderr,"[init_Item] failed to allocate memory.");
        return NULL;
	}
    //set members
    ret_value->symbol = symbol;
    ret_value->y_position = y;
    ret_value->x_position = x;
    //set function pointers
    //function calls
    
    return ret_value;
}
