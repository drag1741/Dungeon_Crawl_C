//utility.h
//
//various utility functions

#ifndef UTILITY_H
#define UTILITY_H

#include "dungeon.h"
#include "character.h"

//initialize stdscr
void init_stdscr();

//set the color pairs
void set_colors();

//keyboard input handler, returns input
int input_handle(struct Dungeon *dungeon, struct Character *player, int input);

#endif
