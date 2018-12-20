//tile.c
//
//implementation of tile struct 
//basic tile struct for map

#include <stdlib.h>
#include <stdbool.h>
#include "tile.h"

//initialize the tile
struct Tile * init_Tile(const int symbol){
    struct Tile *ret_value = malloc(sizeof (struct Tile));
    if( ret_value == NULL) //malloc failed
        return NULL;
    //set members
    ret_value->symbol = symbol;//during init symbol is always '#'
    ret_value->lit = false;
    ret_value->revealed = false;
    ret_value->can_pass_light = false;//changed in floor generate
    ret_value->walkable = false;
    //set function pointers

    //function calls

    return ret_value;
}
