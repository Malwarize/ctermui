#ifndef CTERMUI_H 
#define CTERMUI_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

typedef struct winsize winsize;
typedef struct ctermui{
    char** buffer;
    winsize size;
}*ctermui;

winsize __get_terminal_size();
ctermui ctermui_new();
void ctermui_init(ctermui c);
void ctermui_display_buffer(ctermui c);
void event_loop(ctermui c);
void ctermui_set_char(ctermui c, unsigned int row, unsigned int col, char ch);
void ctermui_set_string(ctermui c, unsigned int row, unsigned int col, char* str);
void ctermui_line(ctermui c, unsigned int row, unsigned int col, unsigned int length, char ch, unsigned int orientation);

#endif