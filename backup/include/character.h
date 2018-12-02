//character.h
//header for character struct

#ifndef CHARACTER_H
#define CHARACTER_H


#include "floor.h"

#define CAN_MOVE_SIZE 30

struct Character{
    //members
    char symbol;//representative symbol
    int can_move_onto[CAN_MOVE_SIZE];//chars that character can move onto
    int y_position;//current y position
    int x_position;//current x position

    //functions
    void (*move_character)(struct Character *, const int, const struct Floor *);
    int (*allowed_to_move)(const struct Character *, int);
    void (*print_character)(const struct Character *);
};

//initialize character: set symbol and x and y positions
struct Character* init_character(const char symbol, const int y, const int x);

//moves the character y_position and x_position
void move_character_imp(struct Character *character, const int input, const struct Floor *floor);

//returns 1 if allowed to move, 0 if not
int allowed_to_move_imp(const struct Character *character, int symbol);

//print character symbol
void print_character_imp(const struct Character *character);

#endif
