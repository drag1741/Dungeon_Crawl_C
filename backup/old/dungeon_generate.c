//dungeon_generate.c
//
//main.c like file for generating dungeon txt files

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "dungeon.h"
#include "utility.h"

#define FLOOR_HEIGHT 20
#define FLOOR_WIDTH 40
#define DEBUG_DUNG_GEN_C 1

int main(int argc, char *argv[]){
    if(argc < 4){
        printf("[usage] %s \%d:height \%d:width \%f:density\n", argv[0]);
        exit(-1);
    }
    //initialize stdscr
    init_stdscr();
    set_colors();

    //initialize dungeon
    struct Dungeon floor;
    floor.height = atoi(argv[1]);
    floor.width = atoi(argv[2]);
    floor.density_percent = atof(argv[3]);
    generate_random_floor(&floor);

    //print out for checking
    if(DEBUG_DUNG_GEN_C){
       print_dungeon(floor);
       refresh();
    }

    //main game loop
    int input = 0;
    while((input = getch()) != 'q'){
    }

    //destroy dungeon
    //delete_dungeon(&floor);//bugged currently
    endwin();
    return 0;
}
