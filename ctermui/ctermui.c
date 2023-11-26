#include <sys/ioctl.h>
#include <unistd.h>
#include "ctermui.h"
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>

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

void start(ctermui c){
    while(1){
        clearScreen();
        ctermui_display_buffer(c);

        sleep(1);
    }
}

void __add_widget(ctermui c, ctermui_widget_t w){
    ctermui_widget_t* widgets = malloc(sizeof(ctermui_widget_t) * (sizeof(c->widgets) + 1));
}

void __ctermui_set_char(ctermui c, unsigned int row, unsigned int col, char ch){
    // check if the cell is out of bounds
    if (row > c->size.ws_row || col > c->size.ws_col){
        printf("ERROR: you are trying to draw cell on row:%d, col:%d which is out of bounds terminal dimensions\n", row, col);
        exit(EXIT_FAILURE);
    }
    if (c->buffer[row][col] != EMPTY){
        // panic cell is not empty
        printf("ERROR: you are trying to draw cell on row:%d, col:%d which is not empty\n", row, col);
        exit(EXIT_FAILURE);
    }
    c->buffer[row][col] = ch;
}

void __ctermui_text(ctermui c, unsigned int row, unsigned int col, char* str){
    for(int i = 0; str[i] != '\0'; i++){
        __ctermui_set_char(c, row, col + i, str[i]);
    }
}
const unsigned int HORIZONTAL = 0;
const unsigned int VERTICAL = 1;

void __ctermui_line(ctermui c, unsigned int row, unsigned int col, unsigned int length, char ch, unsigned int orientation){
    for(int i = 0; i < length; i++){
        if(orientation == HORIZONTAL){
           __ctermui_set_char(c, row, col + i, ch);
        }else{
            __ctermui_set_char(c, row + i, col, ch);
        }
    }
}


void ctermui_charrelative(ctermui c, unsigned int row_relative, unsigned int col_relative, char str){
    __ctermui_set_char(c, (c->size.ws_row * row_relative) / 100 , (c->size.ws_col * col_relative) / 100, str);
}

void ctermui_text_relative(ctermui c, unsigned int row_relative, unsigned int col_relative, char* str){
    __ctermui_text(c, (c->size.ws_row * row_relative) / 100, (c->size.ws_col * col_relative) / 100, str);
}

void ctermui_line_relative(ctermui c, unsigned int row_relative, unsigned int col_relative, unsigned int length_relative, char ch, unsigned int orientation){
    unsigned int length;
    if(orientation == HORIZONTAL){
        length = (c->size.ws_col*length_relative)/100;
    }else{
        length = (c->size.ws_row*length_relative)/100;
    }
    __ctermui_line(c, (c->size.ws_row * row_relative) / 100, (c->size.ws_col * col_relative) / 100, length, ch, orientation);
}

void ctermui_charabsolute(ctermui c, unsigned int row, unsigned int col, char ch){
    __ctermui_set_char(c, row, col, ch);
}
void ctermui_text_absolute(ctermui c, unsigned int row, unsigned int col, char* str){
    __ctermui_text(c, row, col, str);
}
void ctermui_line_absolute(ctermui c, unsigned int row, unsigned int col, unsigned int length, char ch, unsigned int orientation){
    __ctermui_line(c, row, col, length, ch, orientation);
}

ctermui_widget_t ctermui_widget_new(unsigned int row, unsigned int col, unsigned int width, unsigned int height){
    ctermui_widget_t w = malloc(sizeof(struct ctermui_widget_t));
    w->row = row;
    w->col = col;
    w->width = width;
    w->height = height;
    return w;
}

const unsigned char TOP_LEFT_CORNER = '+';
const unsigned char TOP_RIGHT_CORNER = '+';
const unsigned char BOTTOM_LEFT_CORNER = '+';
const unsigned char BOTTOM_RIGHT_CORNER = '+';
const unsigned char HORIZONTAL_LINE = '-';
const unsigned char VERTICAL_LINE = '|';




void ctermui_widget(ctermui c, ctermui_widget_t w){

    ctermui_charabsolute(c, w->row, w->col, TOP_LEFT_CORNER);
    ctermui_charabsolute(c, w->row, w->col + w->width, TOP_RIGHT_CORNER);
    ctermui_charabsolute(c, w->row + w->height, w->col, BOTTOM_LEFT_CORNER);
    ctermui_charabsolute(c, w->row + w->height, w->col + w->width, BOTTOM_RIGHT_CORNER);

    ctermui_line_absolute(c, w->row, w->col + 1, w->width - 1, HORIZONTAL_LINE, HORIZONTAL);
    ctermui_line_absolute(c, w->row + w->height, w->col + 1, w->width - 1, HORIZONTAL_LINE, HORIZONTAL);
    ctermui_line_absolute(c, w->row + 1, w->col, w->height - 1, VERTICAL_LINE, VERTICAL);
    ctermui_line_absolute(c, w->row + 1, w->col + w->width, w->height - 1, VERTICAL_LINE, VERTICAL);
}
// 
// typedef struct ctermui_ctermui_text_box_t{
//     char* text;
//     unsigned int width;
//     unsigned int height;
// }*text_box_t;

ctermui_text_box_t ctermui_text_box_new(ctermui c, ctermui_widget_t w, char* str){
    ctermui_text_box_t t = malloc(sizeof(struct ctermui_text_box_t));
    if(strlen(str) > w->width){
        printf("ERROR: text is too long for the widget\n");
        exit(EXIT_FAILURE);
    }
    if (w->height < 1){
        printf("ERROR: widget is too small for the text box\n");
        exit(EXIT_FAILURE);
    }
    t->text = str;
    t->width = w->width;
    t->height = w->height;
    return t;
}

void ctermui_text_box(ctermui c, ctermui_widget_t w, char* str){
    ctermui_text_box_t t = ctermui_text_box_new(c, w, str);

    ctermui_widget(c, w);
    ctermui_text_absolute(c, w->row + (w->height / 2), w->col + (w->width / 2) - (strlen(str) / 2), str);
}