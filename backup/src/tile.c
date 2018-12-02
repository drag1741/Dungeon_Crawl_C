//tile.c
//
//implementation of tile struct 
//basic tile struct for map

#include <stdlib.h>
#include "tile.h"

//initialize the tile
struct Tile * init_tile(const int symbol){
    struct Tile *ret_value = malloc(sizeof (struct Tile));
    if( ret_value == NULL) //malloc failed
        return NULL;
    //set members
    ret_value->symbol = symbol;
    //set function pointers

    //function calls

    return ret_value;
}
