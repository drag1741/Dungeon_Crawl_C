//tile.c
//
//implementation of tile struct 
//basic tile struct for map

#include <stdlib.h>
#include <stdbool.h>
#include "tile.h"

//initialize the tile
struct Tile * init_tile(const int symbol){
    struct Tile *ret_value = malloc(sizeof (struct Tile));
    if( ret_value == NULL) //malloc failed
        return NULL;
    //set members
    ret_value->symbol = symbol;
    ret_value->lit = false;
    ret_value->revealed = false;
    if(ret_value->symbol == '#'){//set tile to block light passing through
        ret_value->can_pass_light = false;
    }
    else{
        ret_value->can_pass_light = true;
    }
    //set function pointers

    //function calls

    return ret_value;
}
