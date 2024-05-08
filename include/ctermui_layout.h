#ifndef CTERMUI_layout_H
#define CTERMUI_layout_H

#include "ctermui_component.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_layoutS 100
#define MAX_COMPONENTS 10
typedef struct ctermui_layout {
  char id[100];
  uint16_t percentage;
  struct ctermui_layout *children[MAX_layoutS];
  uint16_t type;
  uint16_t children_count;
  ctermui_component_t component[MAX_COMPONENTS];
  uint16_t component_count;
  size_t absolute_x;
  size_t absolute_y;
  size_t absolute_width;
  size_t absolute_height;
} *ctermui_layout_t;
enum ORIENTATION { CTERMUI_HORIZONTAL, CTERMUI_VERTICAL, LEAF };

ctermui_layout_t ctermui_layout_new_root(uint16_t type, size_t width,
                                         size_t height);

ctermui_layout_t ctermui_layout_new(char *id, uint16_t type,
                                    uint16_t percentage);

ctermui_layout_t ctermui_layout_find(ctermui_layout_t root, char *id);

int ctermui_layout_add_child(ctermui_layout_t parent, ctermui_layout_t child);

int ctermui_layout_add_component(ctermui_layout_t layout,
                                 ctermui_component_t ctermui_component);

void ctermui_layout_draw(ctermui_layout_t layout);

void ctermui_calculate_abs_position(ctermui_layout_t root_layout);

ctermui_component_t ctermui_layout_find_component(ctermui_layout_t layout,
                                                  char *id);

#endif // CTERMUI_layout_H
