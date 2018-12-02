//move.c

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "move.h"

#define DEBUG 0

int print_border(char border_char, int delay){
    int x = 0, y = 0;
    int lines, cols;
    getmaxyx(stdscr,lines,cols);
    lines--;
    cols--;
    while(x < cols){
        mvaddch(y,x,'*');
        refresh();
        x++;
        napms(delay);
    }
    while(y < lines){
        mvaddch(y,x,'*');
        refresh();
        y++;
        napms(delay);
    }
    while(x > 0){
        mvaddch(y,x,'*');
        refresh();
        x--;
        napms(delay);
    }
    while(y > 0){
        mvaddch(y,x,'*');
        refresh();
        y--;
        napms(delay);
    }
}

int center(char *title){
    int len = strlen(title);
    int lines, cols;
    getmaxyx(stdscr,lines,cols);
    //find center of screen
    int x = (cols/2) - len/2;
    int y = lines/2;
    mvaddstr(y,x,title);
    refresh();
}

int move_cursor(int input){
    int x, y;
    getyx(stdscr,y,x);
    switch(input){
        case KEY_DOWN:
            y++;
            move(y,x);
            //printw("[Down:%d]",input);
            break;
        case KEY_UP:
            y--;
            move(y,x);
            //printw("[Up:%d]",input);
            break;
        case KEY_LEFT:
            x--;
            move(y,x);
            //printw("[Left:%d]",input);
            break;
        case KEY_RIGHT:
            x++;
            move(y,x);
            //printw("[Right:%d]",input);
            break;
        default:
            addch(input);
            break;
    }
    refresh();
}
