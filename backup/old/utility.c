//utility.c
//
//various utility functions

#include <ncurses.h>
#include "utility.h"
#include "character.h"
#include "dungeon.h"
#include "floor.h"

//initialize stdscr
void init_stdscr(){
    initscr();
    raw();
    keypad(stdscr,TRUE);
    noecho();
    curs_set(0);
}

//set the color pairs
void set_colors(){
    if(has_colors()){
       start_color();
       use_default_colors();
       init_pair(1,COLOR_YELLOW, -1);//rock
       init_pair(2,COLOR_BLUE, -1);//water
       init_pair(3,COLOR_WHITE, -1);//floor tile
    }
}

//keyboard input handler, returns input
int input_handle(struct Dungeon *dungeon, struct Character *player, int input){
    //dungeon_char and floor are for readability
    int dungeon_char = dungeon->floors[dungeon->current_floor]->graph[player->y_position][player->x_position];
    int index = 0;//will hold index from floor->get_entrance_index()
    struct Floor *floor = dungeon->floors[dungeon->current_floor];
    //move player
    if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN ||
       input == 'h'      || input == 'l'       || input == 'k'    || input =='j'){
       //start if body
       player->move_character(player, input, floor);
    }
    else if(input == '>' && dungeon_char  == '>' && dungeon->current_floor != dungeon->depth-1){
        //start else if body
        dungeon->current_floor++;
        if(dungeon->current_floor >= dungeon->depth)
            dungeon->current_floor = dungeon->depth - 1;//-1 for array offset in dungeon->floors
        //move player to start of next dungeon floor
        index = floor->get_exit_index(floor, player->y_position, player->x_position);
        floor = dungeon->floors[dungeon->current_floor];//change floor to current_floor
        player->y_position = floor->entrances[index][0];//update player position on new floor
        player->x_position = floor->entrances[index][1];
    }
    else if(input == '<' && dungeon_char  == '<' && dungeon->current_floor != 0){
        //start else if body
        dungeon->current_floor--;
        if(dungeon->current_floor < 0)
            dungeon->current_floor = 0;//start of dungeon->floors
        //move player to exit of next dungeon floor
        index = floor->get_entrance_index(floor, player->y_position, player->x_position);
        floor = dungeon->floors[dungeon->current_floor];//change floor to current_floor
        player->y_position = floor->exits[index][0];//update player position on new floor
        player->x_position = floor->exits[index][1];
    }
    return input;
}
