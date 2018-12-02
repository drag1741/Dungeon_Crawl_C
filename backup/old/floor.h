//floor.h

#ifndef FLOOR_H
#define FLOOR_H

#define NUM_OF_ENTRS 2 //the number each of entrances and exits

struct Floor{
    //members
    int **graph;
    int width;
    int height;
    float density_percent;
    int entrances[NUM_OF_ENTRS][2];//y,x values for entry
    int exits[NUM_OF_ENTRS][2];//y,x values for exit

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

//get the floor height and set floor.height
int get_floor_height_imp(char *filename);

//returns the index for entrances position of y,x
int get_entrance_index_imp(const struct Floor* floor, int y, int x);

//returns the index for exits position of y,x
int get_exit_index_imp(const struct Floor* floor, int y, int x);

#endif
