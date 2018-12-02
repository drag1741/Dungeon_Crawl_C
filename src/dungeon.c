//dungeon.c
//implementation of dungeon struct

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "dungeon.h"
#include "floor.h"
#include "utility.h"

//initialize the dungeon
struct Dungeon* init_dungeon(const int depth){
    //allocate memory
    struct Dungeon *ret_value = malloc(sizeof (struct Dungeon));
    if (ret_value == NULL) //malloc failed
        return NULL;
    //set members
    ret_value->depth = depth; 
    ret_value->floors = malloc(sizeof * ret_value->floors * ret_value->depth);
    ret_value->current_floor = 0;//start at floors[0]
    //set function pointers
    ret_value->generate_random_dungeon = &generate_random_dungeon_imp;
    ret_value->delete_dungeon = &delete_dungeon_imp;
    ret_value->print_current_floor = &print_current_floor_imp ;
    ret_value->set_dungeon_name = &set_dungeon_name_imp ;
    //function calls
    ret_value->generate_random_dungeon(ret_value);
    ret_value->set_dungeon_name(ret_value);
    
    return ret_value;
}

//generate random dungeon
void generate_random_dungeon_imp(struct Dungeon *dungeon){
    int height = 0, width = 0;
    getmaxyx(dungeon_win, height, width);
    for(int i = 0; i < dungeon->depth ; i++){
        dungeon->floors[i] = init_floor( height, width, .75);  
    }
}

//print floor[current_floor]
void print_current_floor_imp(const struct Dungeon *dungeon){
    mvwprintw(info_win,2,1,"%s: level %d",dungeon->name,dungeon->current_floor);
    wrefresh(info_win);
    //calls print_floor(const struct Floor*) from floor.h
    dungeon->floors[dungeon->current_floor]->print_floor(dungeon->floors[dungeon->current_floor]);
}

//get dungeon->name from assets/dungeon_names.txt
void set_dungeon_name_imp(struct Dungeon *dungeon){
    FILE *fd = NULL;
    char name[DUNGEON_NAME_SIZE];
    fd = fopen("./assets/dungeon_names.txt", "r");
    if ( fd == NULL ){
        printf("assets/dungeon_names.txt did not open");
        strncpy(dungeon->name, "Dungeon of Doom", 100);
    }
    else{//get name from file and remove \n
        fgets(dungeon->name, DUNGEON_NAME_SIZE, fd);
        dungeon->name[strcspn(dungeon->name, "\n")] = 0;
        fclose(fd);
    }
}

//delete the dungeon and free memory
void delete_dungeon_imp(struct Dungeon *dungeon){
}
