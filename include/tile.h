//tile.h
//
//basic tile struct for map

#ifndef TILE_H
#define TILE_H

#include <stdbool.h>

struct Tile{
    int symbol;//ascii symbol for ncurses
    bool lit;//set to true to designate it is lit, will 
    bool revealed;//set to true designate that it has been lit 
    bool can_pass_light;//set to true to light to pass through 
};

//initialize the tile
//returns pointer to initialized struct Tile
struct Tile* init_tile(const int symbol);

#endif
