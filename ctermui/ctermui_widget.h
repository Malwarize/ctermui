#ifndef CTERMUI_WIDGET_H
#define CTERMUI_WIDGET_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ctermui_component.h"
enum ORIENTATION {
    CTERMUI_HORIZONTAL,
    CTERMUI_VERTICAL,
    LEAF,
};


typedef struct ctermui_widget {
    uint16_t percentage;
    struct ctermui_widget* children[10];
    uint16_t type;
    uint16_t cc;
    struct ctermui_component* component[10];
    uint16_t component_count;
    int x;
    int y;
    int width;
    int height;

    int absolute_x;
    int absolute_y;
    int absolute_width;
    int absolute_height;

    int align; 
} *ctermui_widget;

ctermui_widget ctermui_widget_new_root(uint16_t type,int width, int height);
ctermui_widget ctermui_widget_new(uint16_t type, uint16_t percentage);
int ctermui_widget_add_child(ctermui_widget parent, ctermui_widget child);
int ctermui_widget_add_component(ctermui_widget widget, ctermui_component ctermui_component);
void ctermui_widget_draw(ctermui_widget widget);
void ctermui_calculate_abs_position(ctermui_widget root_widget);
#endif