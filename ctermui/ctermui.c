#include <sys/ioctl.h>
#include <unistd.h>
#include "ctermui.h"
#include <sys/ioctl.h>
#include <signal.h>


//EMPTY
const char EMPTY = ' ';

winsize __get_terminal_size(){
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w;
}


void clearScreen() {
    system("clear");
}


ctermui ctermui_new(){
    ctermui c = malloc(sizeof(struct ctermui));
    return c;
}

void ctermui_init(ctermui c){
    winsize w = __get_terminal_size();
    c->size = w;
    c->buffer = malloc(sizeof(char*) * c->size.ws_row);
    for(int i = 0; i < c->size.ws_row; i++){
        c->buffer[i] = malloc(sizeof(char) * c->size.ws_col);
    }
    //spaces 
    for(int i = 0; i < c->size.ws_row; i++){
        for(int j = 0; j < c->size.ws_col; j++){
            c->buffer[i][j] = EMPTY;
        }
    }
}

void __resize_buffer(ctermui c){
    winsize w = __get_terminal_size();
    c->size = w;
    c->buffer = realloc(c->buffer, sizeof(char*) * c->size.ws_row);
    for(int i = 0; i < c->size.ws_row; i++){
        c->buffer[i] = realloc(c->buffer[i], sizeof(char) * c->size.ws_col);
    }
}

void ctermui_display_buffer(ctermui c){
    winsize w = __get_terminal_size();
    if (w.ws_row != c->size.ws_row || w.ws_col != c->size.ws_col){
        __resize_buffer(c);
    }

    for(int i = 0; i < w.ws_row; i++){
        for(int j = 0; j < w.ws_col; j++){
            printf("%c", c->buffer[i][j]);
        }
        printf("\n");
    }
}

unsigned int ctermui_get_cols(ctermui c){
    return c->size.ws_col;
}

unsigned int ctermui_get_rows(ctermui c){
    return c->size.ws_row;
}

void event_loop(ctermui c){
    while(1){
        // check for events
        clearScreen();
        ctermui_display_buffer(c);
        sleep(1);
    }
}

void ctermui_set_char(ctermui c, unsigned int row, unsigned int col, char ch){
    // check if the cell is out of bounds
    if (row >= c->size.ws_row || col >= c->size.ws_col){
        printf("ERROR: you are trying to draw cell on %d, %d which is out of bounds terminal dimensions\n", row, col);
        exit(EXIT_FAILURE);
    }
    if (c->buffer[row][col] != EMPTY){
        // panic cell is not empty
        printf("ERROR: you are trying to draw cell on %d, %d which is not empty\n", row, col);
        exit(EXIT_FAILURE);
    }
    c->buffer[row][col] = ch;
}

void ctermui_set_string(ctermui c, unsigned int row, unsigned int col, char* str){
    for(int i = 0; str[i] != '\0'; i++){
        ctermui_set_char(c, row, col + i, str[i]);
    }
}
const unsigned int HORIZONTAL = 0;
const unsigned int VERTICAL = 1;

void ctermui_line(ctermui c, unsigned int row, unsigned int col, unsigned int length, char ch, unsigned int orientation){
    for(int i = 0; i < length; i++){
        if(orientation == HORIZONTAL){
           ctermui_set_char(c, row, col + i, ch);
        }else{
            ctermui_set_char(c, row + i, col, ch);
        }
    }
}

// relatives to the terminal size and percentage
unsigned int ____ctermui_get_cols_relative(ctermui c, unsigned int percentage){
    return (c->size.ws_col * percentage) / 100;
}

unsigned int ctermui_get_rows_relative(ctermui c, unsigned int percentage){
    return (c->size.ws_row * percentage) / 100;
}

void ctermui_set_char_relative(ctermui c, unsigned int row, unsigned int col, char ch){
    ctermui_set_char(c, ctermui_get_rows_relative(c, row), ____ctermui_get_cols_relative(c, col), ch);
}

void ctermui_set_string_relative(ctermui c, unsigned int row, unsigned int col, char* str){
    ctermui_set_string(c, ctermui_get_rows_relative(c, row), __ctermui_get_cols_relative(c, col), str);
}

void ctermui_line_relative(ctermui c, unsigned int row, unsigned int col, unsigned int length, char ch, unsigned int orientation){
    ctermui_line(c, ctermui_get_rows_relative(c, row), __ctermui_get_cols_relative(c, col), length, ch, orientation);
}

