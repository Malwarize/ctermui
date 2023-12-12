#ifndef CTERMUI_WIDGET_H
#define CTERMUI_WIDGET_H
#include "ctermui_component.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ctermui_widget {
  char id[100];
  uint16_t percentage;
  struct ctermui_widget* children[10];
  uint16_t type;
  uint16_t children_count;
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
} * ctermui_widget;
enum ORIENTATION {
  CTERMUI_HORIZONTAL,
  CTERMUI_VERTICAL,
  LEAF
};
ctermui_widget ctermui_widget_new_root(uint16_t type,
                                       int width,
                                       int height);
ctermui_widget ctermui_widget_new(char* id,
                                  uint16_t type,
                                  uint16_t percentage);
ctermui_widget ctermui_widget_find(ctermui_widget root,
                                   char* id);
int ctermui_widget_add_child(ctermui_widget parent,
                             ctermui_widget child);
int ctermui_widget_add_component(
  ctermui_widget widget,
  ctermui_component ctermui_component);
void ctermui_widget_draw(ctermui_widget widget);
void ctermui_calculate_abs_position(
  ctermui_widget root_widget);
ctermui_component ctermui_widget_find_component(
  ctermui_widget widget, char* id);
#endif  // CTERMUI_WIDGET_H