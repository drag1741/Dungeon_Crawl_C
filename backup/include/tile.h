//tile.h
//
//basic tile struct for map

#ifndef TILE_H
#define TILE_H

struct Tile{
    int symbol;//ascii symbol for ncurses
};


//initialize the tile
//returns pointer to initialized struct Tile
struct Tile* init_tile(const int symbol);

#endif
