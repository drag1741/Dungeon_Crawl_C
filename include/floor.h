//floor.h

#ifndef FLOOR_H
#define FLOOR_H

#include "tile.h"
#include "list.h"

#define NUM_OF_ENTRS 2 //the number each of entrances and exits to the floor


struct Floor{
    //members
    struct Tile ***graph;
    int width;
    int height;
    float density_percent;
    int entrances[NUM_OF_ENTRS][2];//y,x values for entry
    int exits[NUM_OF_ENTRS][2];//y,x values for exit
    int max_visibility;//the maximum lit range for characters
	struct List *items;//linked list of items

    //functions
    void (*generate_random_floor)(struct Floor*);
    void (*graph_from_file)(struct Floor*, char*);
    void (*set_floor_entrances)(struct Floor*);
    void (*set_floor_exits)(struct Floor*);
    void (*delete_floor)(struct Floor*);
    void (*print_floor)(struct Floor*);
    int  (*get_floor_height)(char*);
    int  (*get_entrance_index)(const struct Floor*, int, int);
    int  (*get_exit_index)(const struct Floor*, int, int);
    void (*set_tile_lit_true)( struct Floor*, int, int, int);
    void (*set_tile_lit_false)( struct Floor*, int, int, int);
	void (*set_random_items)(struct Floor*);
    void (*set_item_lit_true)(struct Floor*);
	
};

//initialize the struct Floor *floor, sets function pointers in struct
struct Floor* init_floor(const int width, const int height, const float density_percent);

//generate random floor floor
void generate_random_floor_imp(struct Floor *floor);
    
//fill graph from file
void graph_from_file_imp(struct Floor *floor, char *filename);

//set floor entrance coordinates
void set_floor_entrances_imp(struct Floor *floor);

//set floor exit coordinates
void set_floor_exits_imp(struct Floor *floor);

//delete the floor
void delete_floor_imp(struct Floor *floor);

//print graph to stdscr
void print_floor_imp(struct Floor *floor);

//get the floor height and set floor.height when floor is read in from file
int get_floor_height_imp(char *filename);

//returns the index for entrances position of y,x
int get_entrance_index_imp(const struct Floor* floor, int y, int x);

//returns the index for exits position of y,x
int get_exit_index_imp(const struct Floor* floor, int y, int x);

//set tile->lit=true around character with position char_y, char_x
void set_tile_lit_true_imp( struct Floor *floor, int char_y, int char_x, int light_radius);

//set tile->lit=false around character with position char_y, char_x
void set_tile_lit_false_imp( struct Floor *floor, int char_y, int char_x, int light_radius);

//randomly generate items in struct List items
void set_random_items_imp(struct Floor *floor);

//set item->lit to true
void set_item_lit_true_imp(struct Floor *floor);
#endif
