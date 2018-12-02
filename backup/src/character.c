//character.c
//implementation for character struct

#include <ncurses.h>
#include <stdlib.h>
#include "character.h"
#include "floor.h"
#include "tile.h"

//initialize character: set symbol and x and y positions
struct Character* init_character(const char symbol, const int y, const int x){
    //allocate memory
    struct Character *ret_value = malloc(sizeof (struct Character));
    if (ret_value == NULL) //malloc failed
        return NULL;
    //set members
    ret_value->symbol = symbol;
    ret_value->y_position = y;
    ret_value->x_position = x; 
    //set the char that the character can move onto
    ret_value->can_move_onto[0] = '.';
    ret_value->can_move_onto[1] = '<';
    ret_value->can_move_onto[2] = '>';
    ret_value->can_move_onto[3] = 0;//must end in NULL
    //set function pointers
    ret_value->move_character = &move_character_imp;
    ret_value->allowed_to_move = &allowed_to_move_imp;
    ret_value->print_character = &print_character_imp;

    return ret_value;
}

//moves the character y_position and x_position
void move_character_imp(struct Character *character, const int input, const struct Floor *floor){
    if(input == KEY_LEFT || input == 'h'){
        if(character->allowed_to_move(character,floor->graph[character->y_position][character->x_position-1]->symbol))
            character->x_position -= 1;
    }
    else if(input == KEY_UP || input == 'k'){
        if(character->allowed_to_move(character,floor->graph[character->y_position-1][character->x_position]->symbol))
            character->y_position -= 1;
    }
    else if(input == KEY_RIGHT || input == 'l'){
        if(character->allowed_to_move(character,floor->graph[character->y_position][character->x_position+1]->symbol))
            character->x_position += 1;
    }
    else if(input == KEY_DOWN || input == 'j'){
        if(character->allowed_to_move(character,floor->graph[character->y_position+1][character->x_position]->symbol))
            character->y_position += 1;
    }
}

//returns 1 if allowed to move, 0 if not
int allowed_to_move_imp(const struct Character *character, int symbol){
    for(int i = 0 ; character->can_move_onto[i] != 0 ; i++){
        if(character->can_move_onto[i] == symbol)
            return 1;
    }
    return 0;
}

//print character symbol
void print_character_imp(const struct Character *character){
    mvaddch(character->y_position,character->x_position,character->symbol);
    if(character->symbol == '@')//move cursor on top of player
        move(character->y_position,character->x_position);
}
