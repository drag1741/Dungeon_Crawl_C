//main.c
//dungeon exploration game

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "dungeon.h"
#include "character.h"
#include "utility.h"

int main(int argc, char *argv[]){
/*
    if(argc < 4){
        printf("[usage] %s height width density\n", argv[0]);
        exit(-1);
    }
*/  
    time_t t;//seed srand for random generators
    srand(time(&t));
    init_stdscr();//initialize stdscr
    set_colors();//turn on colors

    //initialize Dungeon struct
    struct Dungeon *dungeon = init_dungeon(10);
    if( dungeon == NULL)//failed to initialize
        exit(-1);
    //initialize Player
    struct Character *player=init_character('@',dungeon->floors[0]->entrances[0][0],
                                                dungeon->floors[0]->entrances[0][1]);
    if( player == NULL)//failed to initialize
        exit(-2);

    //print out current state to screen
    dungeon->print_current_floor(dungeon);
    player->print_character(player);

    //main game loop
    int input = 0;
    while((input = getch()) != 'q'){
       clear();
       //input handling
       input_handle(dungeon, player, input);
       //update screen
       dungeon->print_current_floor(dungeon);
       player->print_character(player);
    }

    //close window and program
    dungeon->delete_dungeon(dungeon);
    endwin();
    return(0);
}
