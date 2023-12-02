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

enum ALIGN {
    CTERMUI_ALIGN_LEFT_CENTER,
    CTERMUI_ALIGN_RIGHT_CENTER,
    CTERMUI_ALIGN_CENTER,
    CTERMUI_ALIGN_LEFT_TOP,
    CTERMUI_ALIGN_RIGHT_TOP,
    CTERMUI_ALIGN_TOP,
    CTERMUI_ALIGN_LEFT_BOTTOM,
    CTERMUI_ALIGN_RIGHT_BOTTOM,
    CTERMUI_ALIGN_BOTTOM,
    CTERMUI_ALIGN_TOP_CENTER,
    CTERMUI_ALIGN_BOTTOM_CENTER
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

typedef struct text {
    char text[100];
    int color;
    int bg_color;

    int align;
} Text;

typedef struct Frame{
    int color;
    int bg_color;

} Frame;
typedef struct Background{
    int color;
} Background;

typedef struct button {
    char* text;
    int align;
    int text_color;
    int bg_color;
} Button;

ctermui_component ctermui_new_button(char* text, int align, int text_color, int bg_color);
ctermui_component ctermui_new_text(char* text,int color, int bg_color, int align);
ctermui_component ctermui_new_frame(int color, int bg_color);
ctermui_component ctermui_new_background(int color,int width, int height);
#endif 