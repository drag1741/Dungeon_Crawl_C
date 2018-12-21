//character.h
//header for character struct

#ifndef CHARACTER_H
#define CHARACTER_H


#include "floor.h"
#include "list.h"

struct Character{
    //members
    char symbol;//representative symbol
    int y_position;//current y position
    int x_position;//current x position
    int last_lit_y;//last y position that character was lit
    int last_lit_x;//last x position that character was lit
    int light_radius;//how many tiles around character to lit always 
    int hp;//number of hit points
    int strength;//strength of character
    struct List *inventory;//inventory of items, holds struct Node*
    int gold;//amount of gold the character currently has
    bool show;//set to true to print to screen, used for character shadow 
    
    //functions
    void (*move_player)(struct Character*, const int, struct Floor *);
    void (*move_monster)(struct Character*, struct Character*, const int, struct Floor *);
    void (*print_character)(const struct Character*);
};

//initialize character: set symbol and y and x positions
struct Character* init_Character(const char symbol, const int y, const int x);

//moves the player y_position and x_position, uses keyboard input
void move_player_imp(struct Character *player, const int input, struct Floor *floor);

//moves the player y_position and x_position, uses keyboard input
void move_monster_imp(struct Character *monster, struct Character *player, const int input, struct Floor *floor);

//print character symbol
void print_character_imp(const struct Character *character);

#endif
