//dungeon.c
//implementation of dungeon struct

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "../include/dungeon.h"
#include "../include/floor.h"
#include "../include/utility.h"

//initialize the dungeon
struct Dungeon* init_dungeon(const int depth){
    //allocate memory
    struct Dungeon *ret_value = malloc(sizeof (struct Dungeon));
    if (ret_value == NULL){ //malloc failed
		fprintf(stderr,"[init_dungeon] failed to allocate memory.");
        return NULL;
	}
    //set members
    ret_value->depth = depth; 
    ret_value->floors = malloc(sizeof * ret_value->floors * ret_value->depth);
    ret_value->current_floor = 0;//start at floors[0]
    //set function pointers
    ret_value->generate_random_dungeon = &generate_random_dungeon_imp;
    ret_value->delete_dungeon = &delete_dungeon_imp;
    ret_value->print_current_floor = &print_current_floor_imp ;
    ret_value->set_dungeon_name = &set_dungeon_name_imp ;
    //function calls
    ret_value->generate_random_dungeon(ret_value);
    ret_value->set_dungeon_name(ret_value);
    
    return ret_value;
}

//generate random dungeon
void generate_random_dungeon_imp(struct Dungeon *dungeon){
    int height = 0, width = 0;
    getmaxyx(dungeon_win, height, width);
    for(int i = 0; i < dungeon->depth ; i++){
        dungeon->floors[i] = init_floor( height, width, .75);  
    }
}

//print floor[current_floor]
void print_current_floor_imp(const struct Dungeon *dungeon){
    //calls print_floor(const struct Floor*) from floor.h
    dungeon->floors[dungeon->current_floor]->print_floor(dungeon->floors[dungeon->current_floor]);
}

//get dungeon->name from assets/dungeon_names.txt
void set_dungeon_name_imp(struct Dungeon *dungeon){
	//open file from argv[1]
	FILE *in_file = NULL;
	in_file = fopen("assets/dungeon_names.txt", "r");
	if( in_file == NULL ){//failed to open file
		fprintf(stderr,"[set_dungeon_name_imp] failed to open file\n");
		exit(-1);
	}

	//find number of lines in in_file	
	char in_string[DUNGEON_NAME_SIZE];
	int newline_count = 0;	
	while(  !feof(in_file) ){	
		fgets(in_string, DUNGEON_NAME_SIZE, in_file);	
		newline_count++;
		
	}
	newline_count--;
	int max_lines = newline_count;

	//reset file and count
	rewind(in_file);
	newline_count = 0;

	//find string at line
	int	line_number = rand() % max_lines;
	while( newline_count != line_number ){	
		fgets(in_string, DUNGEON_NAME_SIZE, in_file);	
		newline_count++;
	}
	//remove \n and print out in_string
	in_string[strcspn( in_string, "\n")] = 0;
	strncpy(dungeon->name,in_string,100);

	//close file
	fclose(in_file);
}

//delete the dungeon and free memory
void delete_dungeon_imp(struct Dungeon *dungeon){
	free(dungeon);
}
