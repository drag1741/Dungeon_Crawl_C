/*  File:character.c
    Author:Jay Drage
    Synapse:implementation for character struct
*/

#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/character.h"
#include "../include/floor.h"
#include "../include/tile.h"
#include "../include/utility.h"
#include "../include/list.h"

//initialize character: set symbol and x and y positions
struct Character* init_Character(const char symbol, const int y, const int x){
    //allocate memory
    struct Character *ret_value = malloc(sizeof (struct Character));
    if (ret_value == NULL){ //malloc failed
        fprintf(stderr,"[init_character] malloc failed.\n");
        return NULL;
    }
    //set members
    ret_value->symbol = symbol;
    ret_value->y_position = y;
    ret_value->x_position = x; 
    ret_value->last_lit_y = y;
    ret_value->last_lit_x = x;
    ret_value->light_radius = 1; 
    ret_value->inventory = init_List(); 
    ret_value->gold = 0; 
    ret_value->hp = 10; 
    ret_value->strength = 5; 
    ret_value->show = false; 
    //set function pointers
    ret_value->move_player = &move_player_imp;
    ret_value->move_monster = &move_monster_imp;
    ret_value->print_character = &print_character_imp;

    return ret_value;
}

//moves the player y_position and x_position
//checks tile->walkable to see if the player is allowed to move onto the tile 
void move_player_imp(struct Character *player, const int input, struct Floor *floor){
    int y = player->y_position;
    int x = player->x_position;
    int new_y = player->y_position;
    int new_x = player->x_position;
    if(input == KEY_LEFT || input == 'h'){//move left
        if(floor->graph[y][x-1]->walkable == true){
            //floor->set_tile_lit_true(floor, *y, *x-1, player->light_radius);
            new_x -= 1;
        }
    }
    else if(input == KEY_UP || input == 'k'){//move up 
        if(floor->graph[y-1][x]->walkable == true){
            //floor->set_tile_lit_true(floor, *y-1, *x, player->light_radius);
            new_y -= 1;
        }
    }
    else if(input == KEY_RIGHT || input == 'l'){//move right
        if(floor->graph[y][x+1]->walkable == true){
            //floor->set_tile_lit_true(floor, *y, *x+1, player->light_radius);
            new_x += 1;
        }
    }
    else if(input == KEY_DOWN || input == 'j'){//move down 
        if(floor->graph[y+1][x]->walkable == true){
           // floor->set_tile_lit_true(floor, *y+1, *x, player->light_radius);
            new_y += 1;
        }
    }
    /*****check for monster collision*****/
    struct Node *cur_node = floor->monsters->head;//current_node
    struct Character *monster = NULL;
    while(cur_node != NULL){
        monster = (struct Character*)cur_node->data;
		if(floor->graph[monster->y_position][monster->x_position] == floor->graph[new_y][new_x]){
            monster->hp -= player->strength;
            return;
		}
		cur_node = cur_node->next;
    }
    player->y_position = new_y;
    player->x_position = new_x;
}
//moves the monster y_position and x_position
//checks tile->walkable to see if the monster is allowed to move onto the tile 
void move_monster_imp(struct Character *monster, struct Character *player, const int input, struct Floor *floor){
    int y = monster->y_position;
    int x = monster->x_position;
    int new_y = monster->y_position;
    int new_x = monster->x_position;
    if(input == KEY_LEFT || input == 'h'){//move left
        if(floor->graph[y][x-1]->walkable == true){
            new_x -= 1;
        }
    }
    else if(input == KEY_UP || input == 'k'){//move up 
        if(floor->graph[y-1][x]->walkable == true){
            new_y -= 1;
        }
    }
    else if(input == KEY_RIGHT || input == 'l'){//move right
        if(floor->graph[y][x+1]->walkable == true){
            new_x += 1;
        }
    }
    else if(input == KEY_DOWN || input == 'j'){//move down 
        if(floor->graph[y+1][x]->walkable == true){
            new_y += 1;
        }
    }
    if(player->y_position == new_y && player->x_position == new_x){
        player->hp -= monster->strength;
    }
    else{
        monster->y_position = new_y;
        monster->x_position = new_x;
    }
}

//print character symbol
void print_character_imp(const struct Character *character){
    //display character position on info_win
    mvwaddch(dungeon_win,character->y_position,character->x_position,character->symbol);
    if(character->symbol == '@')//move cursor on top of player
        move(character->y_position,character->x_position);
}
