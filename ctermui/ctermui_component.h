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
    SOFT_BACKGROUND,
    SOLID_BACKGROUND,
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

typedef struct  {
    char text[100];
    int color;
    int bg_color;

    int align;
} Text;

typedef struct {
    int color;
    int bg_color;

} Frame;

typedef struct {
    int color;
} SoftBackground;

typedef struct {
    int color;
} SolidBackground;

typedef struct {
    char text[100];
    int align;
    int text_color;
    int bg_color;
} Button;

typedef struct{
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
    void (*calculate_absolute_position)(struct ctermui_component* c, int parent_x, int parent_y, int parent_width, int parent_height);
    void* core_component;
} *ctermui_component;

ctermui_component ctermui_new_button(char* id,char* text, int align, int text_color, int bg_color);
ctermui_component ctermui_new_text(char* id,char* text,int color, int bg_color, int align);
ctermui_component ctermui_new_frame(char* id,int color, int bg_color);
ctermui_component ctermui_new_solid_background(char* id,int color,int width, int height);
ctermui_component ctermui_new_soft_background(char* id,int color,int width, int height);
ctermui_component ctermui_new_custom_component(char* id, void (*draw)(ctermui_screen_t s, ctermui_component c));

void ctermui_component_draw_button(ctermui_screen_t s, ctermui_component c);
void ctermui_component_draw_label(ctermui_screen_t s,  ctermui_component c);
void ctermui_component_draw_frame(ctermui_screen_t s, ctermui_component c);
void ctermui_component_draw_solid_background(ctermui_screen_t s, ctermui_component c);
void ctermui_component_draw_soft_background(ctermui_screen_t s, ctermui_component c);
#endif // CTERMUI_COMPONENT_H