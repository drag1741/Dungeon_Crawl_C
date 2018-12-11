/*  File: main.c
    Purpose: dungeon exploration game
    Author: Jay Drage
*/

#include <ncurses.h>
#include <stdlib.h>

#include "dungeon.h"
#include "character.h"
#include "utility.h"

int main(int argc, char *argv[]){
    /*********initialize game struct*********/
    init_game();
    /********initialize Dungeon struct**********/
    struct Dungeon *dungeon = init_dungeon(10);
    if(dungeon == NULL){//failed to initialize
       fprintf(stderr,"[main] dungeon failed to initialize.\n");
       exit(-1);
    }
    /********initialize Player**********/
    struct Character *player=init_character('@',dungeon->floors[0]->entrances[0][0],
                                                dungeon->floors[0]->entrances[0][1]);
    if(player == NULL){//failed to initialize
       fprintf(stderr,"[main] player failed to initialize.\n");
       exit(-2);
    }
    /********main game loop**********/
    int input = 0;
    while(input != 'q'){
       //update screen
       dungeon->print_current_floor(dungeon);
       player->print_character(player);
       wrefresh(dungeon_win);
       wrefresh(info_win);
       //input handling
       input = wgetch(dungeon_win);
       input_handle(dungeon, player, input);
    }
    /**********close window and program************/
    dungeon->delete_dungeon(dungeon);
    endwin();
    return(0);
}
