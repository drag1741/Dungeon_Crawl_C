/* File:tile.h

   Author:Jay Drage
   Synapse:basic tile struct for map
*/

#ifndef TILE_H
#define TILE_H

#include <stdbool.h>

struct Tile{
    int symbol;//ascii symbol for ncurses
    bool lit;//set to true to designate it is lit, will 
    bool revealed;//set to true designate that it has been lit 
    bool can_pass_light;//set to true to enable light to pass through 
    bool walkable;//set to true to let a character walk onto 
};

//initialize the tile
//returns pointer to initialized struct Tile
struct Tile* init_Tile(const int symbol);

#endif
