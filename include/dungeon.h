//dungeon.h

#ifndef DUNGEON_H
#define DUNGEON_H

#include "floor.h"

#define DUNGEON_NAME_SIZE 50
struct Dungeon{
    //members
    char name[DUNGEON_NAME_SIZE];
    struct Floor **floors;//points to array of floors
    int depth;//number of floors
    int current_floor;//index of current_floor in floors
    

    //functions
    void (*generate_random_dungeon)(struct Dungeon *);
    void (*print_current_floor)(const struct Dungeon *);
    void (*set_dungeon_name)(struct Dungeon *);
    void (*delete_dungeon)(struct Dungeon *);
};

//initialize the dungeon
struct Dungeon* init_dungeon(const int depth);

//generate random dungeon
void generate_random_dungeon_imp(struct Dungeon *dungeon);

//print floor[current_floor]
void print_current_floor_imp(const struct Dungeon *dungeon);

//get dungeon->name from assets/dungeon_names.txt
void set_dungeon_name_imp(struct Dungeon *dungeon);

//delete the dungeon
void delete_dungeon_imp(struct Dungeon *dungeon);

#endif
