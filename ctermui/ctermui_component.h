#ifndef CTERMUI_COMPONENT_H
#define CTERMUI_COMPONENT_H
typedef struct ctermui_screen *ctermui_screen_t;
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ctermui_pencil.h"

enum CTYPES {
    TEXT,
    BUTTON,
    FRAME,
    BACKGROUND,
    CUSTOM
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
    char text[100];
    int align;
    int text_color;
    int bg_color;
} Button;
typedef struct custom {
    // TODO: implement custom component
} Custom;

typedef struct ctermui_component {
    char id[100];
    uint16_t type;
    int x;
    int y;
    int width;
    int height;

    int absolute_x;
    int absolute_y;
    int absolute_width;
    int absolute_height;
    void (*draw)(ctermui_screen_t s, struct ctermui_component* c); 
    void* core_component;
} *ctermui_component;

ctermui_component ctermui_new_button(char* id,char* text, int align, int text_color, int bg_color);
ctermui_component ctermui_new_text(char* id,char* text,int color, int bg_color, int align);
ctermui_component ctermui_new_frame(char* id,int color, int bg_color);
ctermui_component ctermui_new_background(char* id,int color,int width, int height);

void ctermui_component_draw_button(ctermui_screen_t s, ctermui_component c);
void ctermui_component_draw_label(ctermui_screen_t s,  ctermui_component c);
void ctermui_component_draw_frame(ctermui_screen_t s, ctermui_component c);
void ctermui_component_draw_background(ctermui_screen_t s, ctermui_component c);

#endif // CTERMUI_COMPONENT_H