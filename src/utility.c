//utility.c
//
//various utility functions

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "../include/utility.h"
#include "../include/character.h"
#include "../include/dungeon.h"
#include "../include/floor.h"
#include "../include/item.h"

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
	   //init custom colors
	   init_color(COLOR_BRIGHT_YELLOW, 1000, 1000, 0);
	   //init color pairs
       init_pair(1,COLOR_YELLOW, -1);//rock
       init_pair(2,COLOR_BLUE, -1);//water,floor tile
       init_pair(3,COLOR_WHITE, -1);//revealed but not lit
       init_pair(4,COLOR_BRIGHT_YELLOW, -1);//revealed but not lit
    }
}

//keyboard input handler, returns input
int input_handle(struct Dungeon *dungeon, struct Character *player, int input){
    input = wgetch(dungeon_win);//get input from player
    //dungeon_char and floor are for readability
    int dungeon_char = dungeon->floors[dungeon->current_floor]->graph[player->y_position][player->x_position]->symbol;
    struct Floor *floor = dungeon->floors[dungeon->current_floor];
    int index = 0;//will hold index from floor->get_entrance_index()
    /*******player movement*********/
    if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN ||
       input == 'h'      || input == 'l'       || input == 'k'    || input =='j'){
       //start if body
       floor->set_tile_lit_false(floor, player->y_position, player->x_position, player->light_radius);//update tile lit and revealed flags to false
       player->move_player(player, input, floor);
    }
    else if(input == '>' && dungeon_char  == '>' && dungeon->current_floor != dungeon->depth-1){
        //start else if body
        floor->set_tile_lit_false(floor, player->y_position, player->x_position, player->light_radius);//update tile lit and revealed flags to false
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
        floor->set_tile_lit_false(floor, player->y_position, player->x_position, player->light_radius);//update tile lit and revealed flags to false
        dungeon->current_floor--;
        //move player to exit of next dungeon floor
        index = floor->get_entrance_index(floor, player->y_position, player->x_position);
        floor = dungeon->floors[dungeon->current_floor];//change floor to current_floor
        player->y_position = floor->exits[index][0];//update player position on new floor
        player->x_position = floor->exits[index][1];
        wclear(dungeon_win);
    }
    else if(input == 'p'){//pick up item
        /*****print floor->items to dungeon_win*****/
        struct Node *cur_node = floor->items->head;//current_node
        struct Item *cur_item = NULL;
        while(cur_node != NULL){
            cur_item = (struct Item*)cur_node->data;
            //move through floor->items and if an item position matches the player position
            //remove from floor->items and add to player->inventory
            if(  cur_item->y_position == player->y_position 
              && cur_item->x_position == player->x_position){
                floor->items->remove_node_mid(floor->items, cur_node);
                player->inventory->add_node_end(player->inventory, cur_node);
                if(cur_item->symbol == '$') player->gold += 50;
            }
            cur_node = cur_node->next;
        }
    }
    return input;
}
//update the screen state and refresh
void update_screen(struct Dungeon *dungeon, struct Character *player){
       /******update the game state******/
       struct Floor *current_floor = dungeon->floors[dungeon->current_floor];//for readability
       current_floor->set_tile_lit_true(current_floor, player->y_position, player->x_position, player->light_radius);//update tile lit and revealed flags
       current_floor->set_item_lit_true(current_floor);//update item lit and revealed flags
       /******print to windows******/
       mvwprintw(info_win,2,1,"%s: level %d",dungeon->name,dungeon->current_floor);
       mvwprintw(info_win,3,1,"Player:(%3d,%3d) ",player->y_position,player->x_position);
       wprintw(info_win,"Gold:%4d  ",player->gold);
       wprintw(info_win,"Inventory size:%4d",player->inventory->size);
       dungeon->print_current_floor(dungeon);
       player->print_character(player);
       wrefresh(dungeon_win);
       wrefresh(info_win);
}
