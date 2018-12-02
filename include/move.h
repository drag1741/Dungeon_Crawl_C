//move.h

#ifndef MOVE_H
#define MOVE_H

//print a border around the screen with border_char
//delay is delay between char prints
int print_border(char border_char, int delay);

//print title in the middle of the screen
int center(char *title);

//use arrow keys to move cursor around
int move_cursor(int input);

#endif
