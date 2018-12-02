//dungeon.c
//implementation of dungeon struct

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "dungeon.h"
#include "floor.h"

#define DEBUG_DUNGEON 0


//initialize the dungeon
struct Dungeon* init_dungeon(const int depth){
    //allocate memory
    struct Dungeon *ret_value = malloc(sizeof (struct Dungeon));
    if (ret_value == NULL) //malloc failed
        return NULL;
    //set members
    strcpy(ret_value->name, "Dungeon of Doom");
    ret_value->depth = depth; 
    ret_value->floors = malloc(sizeof * ret_value->floors * ret_value->depth);
    ret_value->current_floor = 0;//start at floors[0]
    //set function pointers
    ret_value->generate_random_dungeon = &generate_random_dungeon_imp;
    ret_value->delete_dungeon = &delete_dungeon_imp;
    ret_value->print_current_floor = &print_current_floor_imp ;
    //function calls
    ret_value->generate_random_dungeon(ret_value);
    
    return ret_value;
}

//generate random dungeon
void generate_random_dungeon_imp(struct Dungeon *dungeon){
    int height = 0, width = 0;
    getmaxyx(stdscr, height, width);
    for(int i = 0; i < dungeon->depth ; i++){
        dungeon->floors[i] = init_floor( height, width, .7);  
    }
}

//print floor[current_floor]
void print_current_floor_imp(const struct Dungeon *dungeon){
    dungeon->floors[dungeon->current_floor]->print_floor(dungeon->floors[dungeon->current_floor]);
}

//delete the dungeon and free memory
void delete_dungeon_imp(struct Dungeon *dungeon){
}
