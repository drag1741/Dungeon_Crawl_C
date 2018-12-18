//item.h
//header for item struct

#ifndef ITEM_H
#define ITEM_H

struct Item{
    //members
    char symbol;//representative symbol
    int y_position;//current y position
    int x_position;//current x position
};

//initialize character: set symbol and x and y positions
struct Item* init_Item(const char symbol, const int y, const int x);

#endif
