/*  File: main.c
    Author: Jay Drage
    Synapse: dungeon exploration game
*/

#include <ncurses.h>
#include <stdlib.h>

#include "../include/dungeon.h"
#include "../include/character.h"
#include "../include/utility.h"

int main(int argc, char *argv[]){
    /*********initialize game struct*********/
    init_game();
    /********initialize Dungeon struct**********/
    struct Dungeon *dungeon = init_dungeon(3);
    if(dungeon == NULL){//failed to initialize
       fprintf(stderr,"[main] dungeon failed to initialize.\n");
       exit(-1);
    }
    /********initialize Player**********/
    struct Character *player=init_Character('@',dungeon->floors[0]->entrances[0][0],
                                                dungeon->floors[0]->entrances[0][1]);
    if(player == NULL){//failed to initialize
       fprintf(stderr,"[main] player failed to initialize.\n");
       exit(-2);
    }
    /********main game loop**********/
    int input = 0;
    while(input != 'q'){
       update_screen(dungeon,player);
       //input handling
       input = input_handle(dungeon, player, input);
       update_game(dungeon);
    }
    /**********close window and program************/
    dungeon->delete_dungeon(dungeon);
    free(player);
    endwin();
    return(0);
}
