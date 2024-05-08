//
// Created by xorbit on 5/8/24.
//
#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"
#include <string.h>

void ctermui_component_draw_button(ctermui_screen_t s, ctermui_component_t c) {
  // TODO: dimension Validation
  Button *button = c->core_component;
  size_t text_width = strlen(button->text);
  size_t frame_width = text_width + 2;
  size_t frame_height = 3;
  ctermui_pencil_solid_background(s->buffer, c->x, c->y, frame_width,
                                  frame_height, button->bg_color);
  ctermui_pencil_draw_text(s->buffer, c->x + (frame_width - text_width) / 2,
                           c->y + (frame_height - 1) / 2, button->text,
                           button->text_color, button->bg_color);
}

void ctermui_button_calculate_absolute_position(ctermui_component_t c,
                                                size_t parent_x,
                                                size_t parent_y,
                                                size_t parent_width,
                                                size_t parent_height) {
  Button *button = c->core_component;
  size_t text_width = strlen(button->text);
  size_t frame_width = text_width + 2;
  size_t frame_height = 3;
  switch (button->align) {
  case CTERMUI_ALIGN_CENTER:
    c->x = parent_x + (parent_width - frame_width) / 2;
    c->y = parent_y + (parent_height - frame_height) / 2;
    break;
  case CTERMUI_ALIGN_LEFT_TOP:
    c->x = parent_x;
    c->y = parent_y;
    break;
  case CTERMUI_ALIGN_RIGHT_TOP:
    c->x = parent_x + parent_width - frame_width;
    c->y = parent_y;
    break;
  case CTERMUI_ALIGN_LEFT_BOTTOM:
    c->x = parent_x;
    c->y = parent_y + parent_height - frame_height;
    break;
  case CTERMUI_ALIGN_RIGHT_BOTTOM:
    c->x = parent_x + parent_width - frame_width;
    c->y = parent_y + parent_height - frame_height;
    break;
  case CTERMUI_ALIGN_TOP:
    c->x = parent_x + (parent_width - frame_width) / 2;
    c->y = parent_y;
    break;
  case CTERMUI_ALIGN_BOTTOM:
    c->x = parent_x + (parent_width - frame_width) / 2;
    c->y = parent_y + parent_height - frame_height;
    break;
  case CTERMUI_ALIGN_TOP_CENTER:
    c->x = parent_x;
    c->y = parent_y + (parent_height - frame_height) / 2;
    break;
  case CTERMUI_ALIGN_BOTTOM_CENTER:
    c->x = parent_x + parent_width - frame_width;
    c->y = parent_y + (parent_height - frame_height) / 2;
    break;
  case CTERMUI_ALIGN_LEFT_CENTER:
    c->x = parent_x;
    c->y = parent_y + (parent_height - frame_height) / 2;
    break;
  case CTERMUI_ALIGN_RIGHT_CENTER:
    c->x = parent_x + parent_width - frame_width;
    c->y = parent_y + (parent_height - frame_height) / 2;
    break;
  default:
    break;
  }
  c->width = frame_width;
  c->height = frame_height;
}

ctermui_component_t ctermui_new_button(char *id, char *text, size_t align,
                                       int8_t text_color, int8_t bg_color) {
  Button *button_component = malloc(sizeof(Button));
  if (button_component == NULL) {
    fprintf(stderr, "Error: could not allocate memory for button component\n");
    exit(EXIT_FAILURE);
  }
  strcpy(button_component->text, text);
  button_component->align = align;
  button_component->text_color = text_color;
  button_component->bg_color = bg_color;
  return ctermui_new_custom_component(
      id, button_component, ctermui_component_draw_button,
      ctermui_button_calculate_absolute_position);
}
