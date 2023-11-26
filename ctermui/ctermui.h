#ifndef CTERMUI_H 
#define CTERMUI_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

typedef struct winsize winsize;
typedef struct ctermui_widget_t{
    unsigned int row;
    unsigned int col;
    unsigned int width;
    unsigned int height;
}*ctermui_widget_t;
typedef struct ctermui{
    char** buffer;
    winsize size;
    ctermui_widget_t* widgets;
}*ctermui;

typedef struct ctermui_text_box_t{
    char* text;
    unsigned int width;
    unsigned int height;
}*ctermui_text_box_t;

winsize __get_terminal_size();
ctermui ctermui_new();
void ctermui_init(ctermui c);
void ctermui_display_buffer(ctermui c);
void start(ctermui c);

void ctermui_charrelative(ctermui c, unsigned int row_relative, unsigned int col_relative, char ch);
void ctermui_text_relative(ctermui c, unsigned int row_relative, unsigned int col_relative, char* str);
void ctermui_line_relative(ctermui c, unsigned int row_relative, unsigned int col_relative, unsigned int length_relative, char ch, unsigned int orientation);
void ctermui_charabsolute(ctermui c, unsigned int row, unsigned int col, char ch);
void ctermui_text_absolute(ctermui c, unsigned int row, unsigned int col, char* str);
void ctermui_line_absolute(ctermui c, unsigned int row, unsigned int col, unsigned int length, char ch, unsigned int orientation);

ctermui_widget_t ctermui_widget_new(unsigned int row, unsigned int col, unsigned int width, unsigned int height);
void ctermui_widget(ctermui c, ctermui_widget_t w); 

ctermui_text_box_t ctermui_text_box_new(ctermui c, ctermui_widget_t w, char* str);
void ctermui_text_box(ctermui c, ctermui_widget_t w, char* str);

#endif