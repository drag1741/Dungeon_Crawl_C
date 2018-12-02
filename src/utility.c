//utility.c
//
//various utility functions

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "utility.h"
#include "character.h"
#include "dungeon.h"
#include "floor.h"

//windows for ncurses
WINDOW *dungeon_win;// dungeon window
WINDOW *info_win;// information box  window

//initial game setup for main.c
void init_game(){
    //seed srand for random generators
    time_t t;
    srand(time(&t));
    //setup screens
    init_stdscr();
    set_colors();

    int stdscr_max_y, stdscr_max_x;
    getmaxyx(stdscr, stdscr_max_y, stdscr_max_x);

    //intialize dungeon_win
    int dungeon_win_y = stdscr_max_y/2 + stdscr_max_y/4;
    int dungeon_win_x = stdscr_max_x;
    if( !(dungeon_win = newwin(dungeon_win_y,dungeon_win_x,0,0)))//window failed
        addstr("Unable to create dungeon_win");
    keypad(dungeon_win,TRUE);

    //initialize info_win
    int info_win_y = stdscr_max_y - dungeon_win_y;
    int info_win_x = stdscr_max_x;
    if( !(info_win = newwin(info_win_y,info_win_x,dungeon_win_y,0)))//window failed
        addstr("Unable to create info_win");
    wborder(info_win,0,0,0,0,0,0,0,0);
    mvwprintw(info_win,1,1,"Information Window");
    wrefresh(info_win);
}

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
    int dungeon_char = dungeon->floors[dungeon->current_floor]->graph[player->y_position][player->x_position]->symbol;
    int index = 0;//will hold index from floor->get_entrance_index()
    struct Floor *floor = dungeon->floors[dungeon->current_floor];
    //move player
    if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN ||
       input == 'h'      || input == 'l'       || input == 'k'    || input =='j'){
       //start if body
       player->move_player(player, input, floor);
    }
    else if(input == '>' && dungeon_char  == '>' && dungeon->current_floor != dungeon->depth-1){
        //start else if body
        dungeon->current_floor++;
        //move player to start of next dungeon floor
        index = floor->get_exit_index(floor, player->y_position, player->x_position);
        floor = dungeon->floors[dungeon->current_floor];//change floor to current_floor
        player->y_position = floor->entrances[index][0];//update player position on new floor
        player->x_position = floor->entrances[index][1];
        wclear(dungeon_win);
    }
    else if(input == '<' && dungeon_char  == '<' && dungeon->current_floor != 0){
        //start else if body
        dungeon->current_floor--;
        //move player to exit of next dungeon floor
        index = floor->get_entrance_index(floor, player->y_position, player->x_position);
        floor = dungeon->floors[dungeon->current_floor];//change floor to current_floor
        player->y_position = floor->exits[index][0];//update player position on new floor
        player->x_position = floor->exits[index][1];
        wclear(dungeon_win);
    }
    return input;
}
