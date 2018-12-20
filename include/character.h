//character.h
//header for character struct

#ifndef CHARACTER_H
#define CHARACTER_H


#include "floor.h"
#include "list.h"

#define CAN_MOVE_SIZE 30//size of can_move_onto array

struct Character{
    //members
    char symbol;//representative symbol
    int can_move_onto[CAN_MOVE_SIZE];//chars that character can move onto
    int y_position;//current y position
    int x_position;//current x position
    int light_radius;//how many tiles around character to lit always 
    struct List *inventory;//inventory of items, holds struct Node*
    int gold;//amount of gold the player currently has
    bool lit;//set to true to designate it as lit
    bool revealed;//set to true to designate it has been lit
    
    //functions
    void (*move_player)(struct Character*, const int, struct Floor *);
    int (*allowed_to_move)(const struct Character*, int);
    void (*print_character)(const struct Character*);
};

//initialize character: set symbol and x and y positions
struct Character* init_character(const char symbol, const int y, const int x);

//moves the player y_position and x_position, uses keyboard input
void move_player_imp(struct Character *player, const int input, struct Floor *floor);

//returns 1 if allowed to move, 0 if not
int allowed_to_move_imp(const struct Character *character, int symbol);

//print character symbol
void print_character_imp(const struct Character *character);

#endif
