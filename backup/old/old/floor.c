//floor.c

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "floor.h"

//initialize the floor
struct Floor* init_floor(const int height, const int width, const float density_percent){
    //allocate memory
    struct Floor *ret_value = malloc(sizeof (struct Floor));
    if (ret_value == NULL) //malloc failed
        return NULL;
    //set members
    ret_value->height = height;
    ret_value->width = width;
    ret_value->density_percent = density_percent;
    //set function pointers
    ret_value->generate_random_floor = &generate_random_floor_imp;
    ret_value->graph_from_file = &graph_from_file_imp;
    ret_value->set_floor_entrances = &set_floor_entrances_imp;
    ret_value->set_floor_exits = &set_floor_exits_imp;
    ret_value->delete_floor = &delete_floor_imp;
    ret_value->print_floor = &print_floor_imp;
    ret_value->get_floor_height = &get_floor_height_imp;
    ret_value->get_entrance_index = &get_entrance_index_imp;
    ret_value->get_exit_index = &get_exit_index_imp;
    //function calls
    ret_value->generate_random_floor(ret_value);
    ret_value->set_floor_entrances(ret_value);
    ret_value->set_floor_exits(ret_value);

    return ret_value;
}

//generate random floor floor
void generate_random_floor_imp(struct Floor *floor){
    //allocate memory
    floor->graph = malloc(sizeof * floor->graph * floor->height);
    for( int i = 0 ; i < floor->height; i++){
        floor->graph[i] = malloc(sizeof * floor->graph[i] * (floor->width+1));
    }
    //fill floor with rock '#'
    int i, j;
    for( i = 0 ; i < floor->height ; i++){
        for( j = 0 ; j < floor->width ; j++){
            floor->graph[i][j] = '#';
        }
        floor->graph[i][j] = 0;//set null terminator
    }
    //create open spaces '.'
    int y, x, start_x, max_y = 0, max_x = 0;
    int max_room_size = 10;//used for max size of room generation 
    int room_size_y;//stores the current size of room y dimension
    int room_size_x;//stores the current size of room x dimension
    float max_possible_open_spaces = (floor->height*floor->width)-//area minux the perimeter
                                        (2*floor->height + 2*(floor->width-2));
    float number_open_spaces = 0;//once density_percent has been reached
    float percent_open_spaces = 0;//stop generating open space
    //keep generating open spaces until density_percent has been reached
    while(percent_open_spaces < floor->density_percent){
        //find starting position
        y = rand() % (floor->height-1) + 1;//values 1 to (height-1)
        x = rand() % (floor->width-1) + 1;//values 1 to (width-1)
        //generate room
        room_size_y = rand() % max_room_size + 1;//between 1 and max_room_size
        room_size_x = rand() % max_room_size + 1;
        max_y = y + room_size_y; 
        if( max_y >= floor->height )//limit y to inside of perimeter
             max_y = floor->height - 1; 
        max_x = x + room_size_x; 
        if( max_x >= floor->width - 1 )//limit x to inside of perimeter
            max_x = floor->width - 1; 
        start_x = x;//store value of x for later for loop
        for( ; y < max_y ; y++){
            for( x = start_x ; x < max_x ; x++){
                if(floor->graph[y][x] != '.'){
                    floor->graph[y][x] = '.';//open space char
                    number_open_spaces++;
                }
            }
        }
        percent_open_spaces = number_open_spaces/max_possible_open_spaces;
    }
}

//fills graph from file
void graph_from_file_imp(struct Floor *floor, char *filename){
    //find the height of the height of the floor for dynamic memory allocation
    floor->height = floor->get_floor_height(filename);
    //open filename
    FILE *fd;
    if(!(fd = fopen(filename, "r")))
        exit(-1);

    //create memory for floor.graph
    floor->graph = malloc(sizeof * floor->graph * floor->height);
    int line_size, temp,i,j;
    fpos_t position;
    for( i = 0 ; i < floor->height; i++){
        line_size = 0;
        temp = 0;
        fgetpos(fd, &position);//get position of start of line
        //find length of line
        while(temp != '\n'){
           temp = fgetc(fd); 
           line_size++;
        }
        fsetpos(fd, &position);//set position to start of line
        floor->graph[i] = malloc(sizeof * floor->graph[i] * line_size); 
        //load characters from file 
        for( j = 0 ; j < line_size-1; j++){
            floor->graph[i][j] = fgetc(fd);
        }
        floor->graph[i][j] = 0;//set null terminator
        fgetc(fd);//throw away '\n'
    }
    fclose(fd);
}

//set floor entrance coordinates
void set_floor_entrances_imp(struct Floor *floor){
    int y = 0, x = 0;
    for( int i = 0; i < NUM_OF_ENTRS ; i++){
        while(floor->graph[y][x] != '.'){
            y = rand() % (floor->height-1) + 1;//values 1 to (height-1)
            x = rand() % (floor->width-1) + 1;//values 1 to (width-1)
        }
        floor->entrances[i][0] = y;
        floor->entrances[i][1] = x;
        floor->graph[y][x] = '<';
    }
}

//set floor exit coordinates
void set_floor_exits_imp(struct Floor *floor){
    int y = 0, x = 0;
    for( int i = 0; i < NUM_OF_ENTRS ; i++){
        while(floor->graph[y][x] != '.'){
            y = rand() % (floor->height-1) + 1;//values 1 to (height-1)
            x = rand() % (floor->width-1) + 1;//values 1 to (width-1)
        }
        floor->exits[i][0] = y;
        floor->exits[i][1] = x;
        floor->graph[y][x] = '>';
    }
}

//get the floor height and set floor.height
int get_floor_height_imp(char *filename){
    //open filename
    FILE *fd;
    fd = fopen(filename, "r");
    if(fd == NULL)
        exit(-1);
    int lines = 0;//count the number of lines
    int ch; 
    while( EOF != (ch = fgetc(fd)))
        if( ch == '\n')
            lines++; 
    fclose(fd);
    return lines;
}

//delete the floor and free memory
void delete_floor_imp(struct Floor *floor){
    for(int i = 0 ; i < floor->height; i++)
        free(floor->graph[i]);
    free(floor->graph);
}

//prints graph to screen
void print_floor_imp(struct Floor *floor){
    int i, j;
    for( i = 0 ; i < floor->height; i++){
        j = 0 ;
        while(floor->graph[i][j] != 0){
            if( floor->graph[i][j] == '#'){//rock
                attron(COLOR_PAIR(1));
                mvaddch(i,j,floor->graph[i][j]);
                attroff(COLOR_PAIR(1));
            }
            else if( floor->graph[i][j] == '~'){//water
                attron(COLOR_PAIR(2));
                mvaddch(i,j,floor->graph[i][j]);
                attroff(COLOR_PAIR(2));
            }
            else if( floor->graph[i][j] == '.'){//floor tile
                attron(COLOR_PAIR(3));
                mvaddch(i,j,floor->graph[i][j]);
                attroff(COLOR_PAIR(3));
            }
            else{
                mvaddch(i,j,floor->graph[i][j]);
            }
            j++;
        }
    }
}

//returns the index for entrances position of y,x
int get_entrance_index_imp(const struct Floor *floor, int y, int x){
    for( int i = 0 ; i < NUM_OF_ENTRS ; i++){
        if(floor->entrances[i][0] == y && floor->entrances[i][1] == x)
            return i;
    }
    return -1;//not found
}

//returns the index for exits position of y,x
int get_exit_index_imp(const struct Floor *floor, int y, int x){
    for( int i = 0 ; i < NUM_OF_ENTRS ; i++){
        if(floor->exits[i][0] == y && floor->exits[i][1] == x)
            return i;
    }
    return -1;//not found
}
