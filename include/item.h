//item.h
//header for item struct

#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>

struct Item{
    //members
    char symbol;//representative symbol
    int y_position;//current y position
    int x_position;//current x position
    bool lit;//set to true to designate it as lit 
    bool revealed;//set to true designate that it has been lit 
};

//initialize character: set symbol and x and y positions
struct Item* init_Item(const char symbol, const int y, const int x);

#endif
