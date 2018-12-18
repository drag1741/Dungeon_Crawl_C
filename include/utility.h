//utility.h
//
//various utility functions

#ifndef UTILITY_H
#define UTILITY_H

#include <ncurses.h>

#include "dungeon.h"
#include "character.h"

//windows for ncurses
extern WINDOW *dungeon_win;
extern WINDOW *info_win;

//initialize game
void init_game();

//initialize stdscr
void init_stdscr();

//set the color pairs
void set_colors();
//ncurses colors
#define COLOR_BRIGHT_YELLOW 8

//keyboard input handler, returns input
int input_handle(struct Dungeon *dungeon, struct Character *player, int input);

//update the screen state and refresh
void update_screen(struct Dungeon *dungeon, struct Character *player);

#endif
