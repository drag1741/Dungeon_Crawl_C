//tile.h
//
//basic tile struct for map

#ifndef TILE_H
#define TILE_H

#include <stdbool.h>

struct Tile{
    int symbol;//ascii symbol for ncurses
    bool show;//set to true to print out and show
};

//initialize the tile
//returns pointer to initialized struct Tile
struct Tile* init_tile(const int symbol);

#endif
