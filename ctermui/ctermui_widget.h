#ifndef CTERMUI_WIDGET_H
#define CTERMUI_WIDGET_H
#include "ctermui_component.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_WIDGETS 100
#define MAX_COMPONENTS 10
typedef struct ctermui_widget {
  char id[100];
  uint16_t percentage;
  struct ctermui_widget* children[MAX_WIDGETS];
  uint16_t type;
  uint16_t children_count;
  ctermui_component_t component[MAX_COMPONENTS];
  uint16_t component_count;
  size_t absolute_x;
  size_t absolute_y;
  size_t absolute_width;
  size_t absolute_height;
}* ctermui_widget_t;
enum ORIENTATION {
  CTERMUI_HORIZONTAL,
  CTERMUI_VERTICAL,
  LEAF
};
ctermui_widget_t ctermui_widget_new_root(uint16_t type,
                                       size_t width,
                                       size_t height);
ctermui_widget_t ctermui_widget_new(char* id,
                                  uint16_t type,
                                  uint16_t percentage);
ctermui_widget_t ctermui_widget_find(ctermui_widget_t root,
                                   char* id);
int ctermui_widget_add_child(ctermui_widget_t parent,
                             ctermui_widget_t child);
int ctermui_widget_add_component(
  ctermui_widget_t widget,
  ctermui_component_t ctermui_component);
void ctermui_widget_draw(ctermui_widget_t widget);
void ctermui_calculate_abs_position(
  ctermui_widget_t root_widget);
ctermui_component_t ctermui_widget_find_component(
  ctermui_widget_t widget, char* id);
#endif  // CTERMUI_WIDGET_H