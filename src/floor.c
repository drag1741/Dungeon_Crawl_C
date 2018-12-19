//floor.c

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>//temporary to test set_random_items

#include "../include/floor.h"
#include "../include/character.h"
#include "../include/tile.h"
#include "../include/utility.h"
#include "../include/list.h"
#include "../include/node.h"
#include "../include/item.h"

//initialize the floor
struct Floor* init_floor(const int height, const int width, const float density_percent){
    //allocate memory
    struct Floor *ret_value = malloc(sizeof (struct Floor));
    if (ret_value == NULL){ //malloc failed
        fprintf(stderr,"[init_floor] malloc failed.\n");
        return NULL;
    }
    //set members
    ret_value->height = height;
    ret_value->width = width;
    ret_value->density_percent = density_percent;
    ret_value->max_visibility = 7;//the maximum lit range for characters
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
    ret_value->set_tile_lit_true = &set_tile_lit_true_imp;
    ret_value->set_tile_lit_false = &set_tile_lit_false_imp;
	ret_value->set_random_items = &set_random_items_imp;
	ret_value->set_item_lit_true = &set_item_lit_true_imp;

    //function calls
    ret_value->generate_random_floor(ret_value);
    ret_value->set_floor_entrances(ret_value);
    ret_value->set_floor_exits(ret_value);
    ret_value->set_random_items(ret_value);

    return ret_value;
}

//generate random floor floor
void generate_random_floor_imp(struct Floor *floor){
    /******allocate memory******/
    floor->graph = malloc(sizeof * floor->graph * floor->height);
    if(floor->graph == NULL){
        fprintf(stderr,"[generate_random_floor_imp]floor->graph malloc failed\n");
        exit(-1);
    }
    for( int i = 0 ; i < floor->height; i++){
        floor->graph[i] = malloc(sizeof(floor->graph[i]) * (floor->width+1));
        if(floor->graph[i] == NULL){
            fprintf(stderr,"[generate_random_floor_imp]floor->graph[%d] malloc failed\n",i);
            exit(-1);
        }
    }
    /******fill floor with rock '#'******/
    int i, j;
    for( i = 0 ; i < floor->height ; i++){
        for( j = 0 ; j < floor->width ; j++){
            floor->graph[i][j] = init_tile('#');
        }
        floor->graph[i][j] = init_tile(0);
    }
    /******create open spaces '.'******/
    int y, x, start_x, max_y = 0, max_x = 0;
    int max_room_size = 10;//used for max size of room generation 
    int room_size_y;//stores the current size of room y dimension
    int room_size_x;//stores the current size of room x dimension
    float max_possible_open_spaces = (floor->height*floor->width)-//area minux the perimeter
                                        (2*floor->height + 2*(floor->width-2));
    float number_open_spaces = 0;//once density_percent has been reached
    float percent_open_spaces = 0;//stop generating open space
    /******keep generating open spaces until density_percent has been reached******/
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
                if(floor->graph[y][x]->symbol != '.'){
                    floor->graph[y][x]->symbol = '.';//open space char
                    floor->graph[y][x]->can_pass_light = true;//allow light to pass through
                    number_open_spaces++;
                }
            }
        }
        percent_open_spaces = number_open_spaces/max_possible_open_spaces;
    }
}

/******fills graph from file******/
void graph_from_file_imp(struct Floor *floor, char *filename){
    //find the height of the height of the floor for dynamic memory allocation
    floor->height = floor->get_floor_height(filename);
    //open filename
    FILE *fd;
    if(!(fd = fopen(filename, "r")))
        exit(-1);

    //create memory for floor.graph
    floor->graph = malloc(sizeof * floor->graph * floor->height);
    if(floor->graph == NULL){
        fprintf(stderr,"[generate_random_floor_imp]floor->graph malloc failed\n");
        exit(-1);
    }
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
        if(floor->graph[i] == NULL){
            fprintf(stderr,"[generate_random_floor_imp]floor->graph[%d] malloc failed\n",i);
            exit(-1);
        }
        //load characters from file 
        for( j = 0 ; j < line_size-1; j++){
            floor->graph[i][j] = init_tile(fgetc(fd));
        }
        floor->graph[i][j] = init_tile(0);
        fgetc(fd);//throw away '\n'
    }
    fclose(fd);
}

//set floor entrance coordinates
void set_floor_entrances_imp(struct Floor *floor){
    int y = 0, x = 0;
    for( int i = 0; i < NUM_OF_ENTRS ; i++){
        while(floor->graph[y][x]->symbol != '.'){
            y = rand() % (floor->height-1) + 1;//values 1 to (height-1)
            x = rand() % (floor->width-1) + 1;//values 1 to (width-1)
        }
        floor->entrances[i][0] = y;
        floor->entrances[i][1] = x;
        floor->graph[y][x]->symbol = '<';
    }
}

//set floor exit coordinates
void set_floor_exits_imp(struct Floor *floor){
    int y = 0, x = 0;
    for( int i = 0; i < NUM_OF_ENTRS ; i++){
        while(floor->graph[y][x]->symbol != '.'){
            y = rand() % (floor->height-1) + 1;//values 1 to (height-1)
            x = rand() % (floor->width-1) + 1;//values 1 to (width-1)
        }
        floor->exits[i][0] = y;
        floor->exits[i][1] = x;
        floor->graph[y][x]->symbol = '>';
    }
}

//get the floor height and set floor.height when floor is read in from file
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

//prints graph to dungeon_win 
void print_floor_imp(struct Floor *floor){
    int i, j;
    for( i = 0 ; i < floor->height; i++){
        j = 0 ;
        while(floor->graph[i][j]->symbol != 0){
            if(floor->graph[i][j]->lit == true){//only print if lit is set true
                if( floor->graph[i][j]->symbol == '#'){//rock
                    wattron(dungeon_win, COLOR_PAIR(1));
                    mvwaddch(dungeon_win,i,j,floor->graph[i][j]->symbol);
                    wattroff(dungeon_win, COLOR_PAIR(1));
                }
                else if( floor->graph[i][j]->symbol == '.'){//floor tile
                    wattron(dungeon_win, COLOR_PAIR(2));
                    mvwaddch(dungeon_win,i,j,floor->graph[i][j]->symbol);
                    wattroff(dungeon_win, COLOR_PAIR(2));
                }
                else{
                    mvwaddch(dungeon_win,i,j,floor->graph[i][j]->symbol);
                }
            }
            else if(floor->graph[i][j]->revealed == true){
                //change color to grey if revealed but not lit
				wattron(dungeon_win, COLOR_PAIR(3));//COLOR_WHITE
				mvwaddch(dungeon_win,i,j,floor->graph[i][j]->symbol);
				wattroff(dungeon_win, COLOR_PAIR(3));
            }
            j++;
        }
    }
    /*****print floor->items to dungeon_win*****/
    struct Node *cur_node = floor->items->head;//current_node
    struct Item *cur_item = NULL;
    while(cur_node != NULL){
        cur_item = (struct Item*)cur_node->data;
		if(cur_item->lit == true){
			wattron(dungeon_win, COLOR_PAIR(4));//COLOR_BRIGHT_YELLOW
			mvwaddch(dungeon_win,cur_item->y_position,cur_item->x_position, cur_item->symbol);
			wattroff(dungeon_win, COLOR_PAIR(4));
		}
		else if(cur_item->revealed == true){
			wattron(dungeon_win, COLOR_PAIR(3));//COLOR_WHITE
			mvwaddch(dungeon_win,cur_item->y_position,cur_item->x_position, cur_item->symbol);
			wattroff(dungeon_win, COLOR_PAIR(3));
		}
		cur_node = cur_node->next;
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

//set tile->lit=true around character with position char_y, char_x
void set_tile_lit_true_imp( struct Floor *floor, int char_y, int char_x, int light_radius){
    struct Tile ***fGraph = floor->graph; 
	int i,j;//iterators for for loops, i is for y-coordinate and j is for x-coordinate
	int char_lit_radius = floor->max_visibility;//radius around character to set tile->lit=false
	int clear_lit_start_x, clear_lit_stop_x, clear_lit_start_y, clear_lit_stop_y;
	int lit_start_x, lit_stop_x, lit_start_y, lit_stop_y;
	lit_start_x = char_x - char_lit_radius;
	lit_stop_x = char_x + char_lit_radius;
	lit_start_y = char_y - char_lit_radius;
	lit_stop_y = char_y + char_lit_radius;
	clear_lit_start_x = lit_start_x - 1;
	clear_lit_stop_x = lit_stop_x + 1;
	clear_lit_start_y = lit_start_y - 1;
	clear_lit_stop_y = lit_stop_y + 1;
    //check lit_start for fGraph out of bounds
    if( lit_start_y < 0 ) lit_start_y = 0;
    if( lit_start_x < 0 ) lit_start_x = 0;
    if( lit_stop_y > floor->height ) lit_stop_y = floor->height;
    if( lit_stop_x > floor->width ) lit_stop_x = floor->width;
    //check clear_lit_start for fGraph out of bounds
    if( clear_lit_start_y < 0 ) clear_lit_start_y = 0;
    if( clear_lit_start_x < 0 ) clear_lit_start_x = 0;
    if( clear_lit_stop_y > floor->height ) clear_lit_stop_y = floor->height;
    if( clear_lit_stop_x > floor->width ) clear_lit_stop_x = floor->width;
    /*******always light radius around character*******/
    int start_y, start_x, max_y, max_x;
    start_y = char_y - light_radius; 
    start_x = char_x - light_radius; 
    max_y = char_y + light_radius + 1;
    max_x = char_x + light_radius + 1;
    //check for graph out of bounds
    if( start_y < 0 ) start_y =0;
    if( start_x < 0 ) start_x =0;
    if( max_y > floor->height ) max_y = floor->height;
    if( max_x > floor->width ) max_x = floor->width;
    //set square around player 
    for( i = char_y-1 ; i < char_y+2 ; i++ )
        for( j = char_x-1 ; j < char_x+2 ; j++ ){
            fGraph[i][j]->lit = true;
            fGraph[i][j]->revealed = true;
    }
	/**********light cross from player********/
	//right
    j = 0;
    while( j < char_lit_radius){
        fGraph[char_y][char_x + j]->lit = true;
        fGraph[char_y][char_x + j]->revealed = true;
        if(fGraph[char_y][char_x + j]->symbol == '#'){//light tile until '#'
            break;
        } 
        j++;
    }
	//down
    i = 0;
    while( i < char_lit_radius){
        fGraph[char_y + i][char_x]->lit = true;
        fGraph[char_y + i][char_x]->revealed = true;
        if(fGraph[char_y + i][char_x]->symbol == '#'){//light tile until '#'
            break;
        } 
        i++;
    }
	//left
    j = 0;
    while( j < char_lit_radius){
        fGraph[char_y][char_x-j]->lit = true;
        fGraph[char_y][char_x-j]->revealed = true;
        if(fGraph[char_y][char_x-j]->symbol == '#'){//light tile until '#'
            break;
        } 
        j++;
    }
	//up
    i = 0;
    while( i < char_lit_radius){
        fGraph[char_y - i][char_x]->lit = true;
        fGraph[char_y - i][char_x]->revealed = true;
        if(fGraph[char_y - i][char_x]->symbol == '#'){//light tile until '#'
            break;
        } 
        i++;
    }
    /*******cast light clockwise starting in upper right******/	
	//light upper right
    for( i = 1; i < char_lit_radius; i++){
        if( char_y - i < 0) break;//out of bounds
        for( j = 1; j < char_lit_radius; j++){
            if( char_x + j > floor->width ) break;//out of bounds
            if((fGraph[char_y - i+1][char_x + j-1]->lit == true)//check bottom left
            && (fGraph[char_y - i+1][char_x + j-1]->can_pass_light == true)){
                /*****/
                fGraph[char_y - i][char_x + j]->lit = true;
                fGraph[char_y - i][char_x + j]->revealed = true;
            }
            else if((i == 1)//only check row just above character
                 && (fGraph[char_y - i+1][char_x + j-1]->lit == true)//check bottom left
                 && (fGraph[char_y - i+1][char_x + j-1]->can_pass_light == false)
                 && (fGraph[char_y - i+1][char_x + j-2]->lit == true)//check bottom 2xleft
                 && (fGraph[char_y - i+1][char_x + j-2]->can_pass_light == true)
                 && (fGraph[char_y - i][char_x + j-1]->lit == true)//check left
                 && (fGraph[char_y - i][char_x + j-1]->can_pass_light == true)){
                 /*****/
                 fGraph[char_y - i][char_x + j]->lit = true;
                 fGraph[char_y - i][char_x + j]->revealed = true;
            }
        }
    }
	//light lower right
    for( i = 1; i < char_lit_radius; i++){
        if( char_y + i > floor->height) break;//out of bounds
        for( j = 1; j < char_lit_radius; j++){
            if( char_x + j > floor->width ) break;//out of bounds
            if((fGraph[char_y + i-1][char_x + j-1]->lit == true)//check bottom left
            && (fGraph[char_y + i-1][char_x + j-1]->can_pass_light == true)){
                /*****/
                fGraph[char_y + i][char_x + j]->lit = true;
                fGraph[char_y + i][char_x + j]->revealed = true;
            }
            else if((i == 1)//only check row just above character
                 && (fGraph[char_y + i-1][char_x + j-1]->lit == true)//check bottom left
                 && (fGraph[char_y + i-1][char_x + j-1]->can_pass_light == false)
                 && (fGraph[char_y + i-1][char_x + j-2]->lit == true)//check bottom 2xleft
                 && (fGraph[char_y + i-1][char_x + j-2]->can_pass_light == true)
                 && (fGraph[char_y + i][char_x + j-1]->lit == true)//check left
                 && (fGraph[char_y + i][char_x + j-1]->can_pass_light == true)){
                 /*****/
                 fGraph[char_y + i][char_x + j]->lit = true;
                 fGraph[char_y + i][char_x + j]->revealed = true;
            }
        }
    }
	//light upper left
    for( i = 1; i < char_lit_radius; i++){
        if( char_y - i < 0) break;//out of bounds
        for( j = 1; j < char_lit_radius; j++){
            if( char_x - j < 0 ) break;//out of bounds
            if((fGraph[char_y - i+1][char_x - j+1]->lit == true)//check bottom right 
            && (fGraph[char_y - i+1][char_x - j+1]->can_pass_light == true)){
                /*****/
                fGraph[char_y - i][char_x - j]->lit = true;
                fGraph[char_y - i][char_x - j]->revealed = true;
            }
            else if((i == 1)//only check row just above character
                 && (fGraph[char_y - i+1][char_x - j+1]->lit == true)//check bottom right 
                 && (fGraph[char_y - i+1][char_x - j+1]->can_pass_light == false)
                 && (fGraph[char_y - i+1][char_x - j+2]->lit == true)//check bottom 2xright
                 && (fGraph[char_y - i+1][char_x - j+2]->can_pass_light == true)
                 && (fGraph[char_y - i][char_x - j+1]->lit == true)//check right 
                 && (fGraph[char_y - i][char_x - j+1]->can_pass_light == true)){
                 /*****/
                 fGraph[char_y - i][char_x - j]->lit = true;
                 fGraph[char_y - i][char_x - j]->revealed = true;
            }
        }
    }
	//light lower left
    for( i = 1; i < char_lit_radius; i++){
        if( char_y + i > floor->height) break;//out of bounds
        for( j = 1; j < char_lit_radius; j++){
            if( char_x - j < 0 ) break;//out of bounds
            if((fGraph[char_y + i-1][char_x - j+1]->lit == true)//check bottom right 
            && (fGraph[char_y + i-1][char_x - j+1]->can_pass_light == true)){
                /*****/
                fGraph[char_y + i][char_x - j]->lit = true;
                fGraph[char_y + i][char_x - j]->revealed = true;
            }
            else if((i == 1)//only check row just above character
                 && (fGraph[char_y + i-1][char_x - j+1]->lit == true)//check bottom right 
                 && (fGraph[char_y + i-1][char_x - j+1]->can_pass_light == false)
                 && (fGraph[char_y + i-1][char_x - j+2]->lit == true)//check bottom 2xright
                 && (fGraph[char_y + i-1][char_x - j+2]->can_pass_light == true)
                 && (fGraph[char_y + i][char_x - j+1]->lit == true)//check right 
                 && (fGraph[char_y + i][char_x - j+1]->can_pass_light == true)){
                 /*****/
                 fGraph[char_y + i][char_x - j]->lit = true;
                 fGraph[char_y + i][char_x - j]->revealed = true;
            }
        }
    }
}

//set tile->lit=false around character with position char_y, char_x
void set_tile_lit_false_imp( struct Floor *floor, int char_y, int char_x, int light_radius){
    /*******set previous tile->lit = false, keep tile->revealed=true******/
    struct Tile ***fGraph = floor->graph; 
	int i,j;//iterators for for loops, i is for y-coordinate and j is for x-coordinate
	int char_lit_radius = floor->max_visibility;//radius around character to set tile->lit=false
	int clear_lit_start_x, clear_lit_stop_x, clear_lit_start_y, clear_lit_stop_y;
	int lit_start_x, lit_stop_x, lit_start_y, lit_stop_y;
	lit_start_x = char_x - char_lit_radius;
	lit_stop_x = char_x + char_lit_radius;
	lit_start_y = char_y - char_lit_radius;
	lit_stop_y = char_y + char_lit_radius;
	clear_lit_start_x = lit_start_x - 1;
	clear_lit_stop_x = lit_stop_x + 1;
	clear_lit_start_y = lit_start_y - 1;
	clear_lit_stop_y = lit_stop_y + 1;
    //check lit_start for fGraph out of bounds
    if( lit_start_y < 0 ) lit_start_y = 0;
    if( lit_start_x < 0 ) lit_start_x = 0;
    if( lit_stop_y > floor->height ) lit_stop_y = floor->height;
    if( lit_stop_x > floor->width ) lit_stop_x = floor->width;
    //check clear_lit_start for fGraph out of bounds
    if( clear_lit_start_y < 0 ) clear_lit_start_y = 0;
    if( clear_lit_start_x < 0 ) clear_lit_start_x = 0;
    if( clear_lit_stop_y > floor->height ) clear_lit_stop_y = floor->height;
    if( clear_lit_stop_x > floor->width ) clear_lit_stop_x = floor->width;
	//set tile->lit = false
    for( i = clear_lit_start_y ; i < clear_lit_stop_y ; i++ )
        for( j = clear_lit_start_x ; j < clear_lit_stop_x ; j++ ){
            fGraph[i][j]->lit = false;
    }
}

//randomly generate items in struct List *items
void set_random_items_imp(struct Floor *floor){
    enum Type type = item;
	floor->items = init_List();
    //find random tile to position item
    int y = rand() % (floor->height-1) + 1;//values 1 to (height-1)
    int x = rand() % (floor->width-1) + 1;//values 1 to (width-1)
    for(int i = 0; i < 5; i++){
        /****find y and x positions for item****/
        while(floor->graph[y][x]->symbol != '.'){
            y = rand() % (floor->height-1) + 1;//values 1 to (height-1)
            x = rand() % (floor->width-1) + 1;//values 1 to (width-1)
        }
        struct Item *item = init_Item('$',y,x);
        struct Node *node = init_Node(item,type); 
        floor->items->add_node_end(floor->items,node);//add to floor->items
        y = rand() % (floor->height-1) + 1;//values 1 to (height-1)
        x = rand() % (floor->width-1) + 1;//values 1 to (width-1)
    }
}
//set item->lit to true
void set_item_lit_true_imp(struct Floor *floor){
    struct Node *cur_node = floor->items->head;//current_node
    struct Item *cur_item = NULL;
    while(cur_node != NULL){
        cur_item = (struct Item*)cur_node->data;
		if(floor->graph[cur_item->y_position][cur_item->x_position]->lit == true){
            cur_item->lit = true;
            cur_item->revealed = true;
		}
        else{
            cur_item->lit = false;
        }
		cur_node = cur_node->next;
    }
}
