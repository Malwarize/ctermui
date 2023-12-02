#ifndef CTERMUI_COMPONENT_H
#define CTERMUI_COMPONENT_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum CTYPES {
    TEXT,
    BUTTON,
    FRAME,
    BACKGROUND
};

typedef struct ctermui_component {
    uint16_t type;
    int x;
    int y;
    int width;
    int height;

    int absolute_x;
    int absolute_y;
    int absolute_width;
    int absolute_height;

    void* core_component;
} *ctermui_component;

typedef struct button {
    char text[100];
    void (*on_click)(void);
} Button;

typedef struct text {
    char text[100];
    int color;
    int bg_color;
} Text;

typedef struct Frame{
    int color;
    int bg_color;
} Frame;
typedef struct Background{
    int color;
} Background;
ctermui_component ctermui_new_button(char* text, void (*on_click)(void));
ctermui_component ctermui_new_text(char* text,int color, int bg_color);
ctermui_component ctermui_new_frame(int color, int bg_color, int width, int height);
ctermui_component ctermui_new_background(int color,int width, int height);
#endif 