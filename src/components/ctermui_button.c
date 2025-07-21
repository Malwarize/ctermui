//
// Created by xorbit on 5/8/24.
//
#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"
#include <string.h>

/**
 * @brief Draw a button component on the screen.
 *
 * Draws the button's background and centers the text within the button frame.
 *
 * @param s Pointer to the screen structure.
 * @param c Pointer to the button component.
 */
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

/**
 * @brief Calculate the absolute position and size of a button component.
 *
 * Sets the x, y, width, and height fields of the button based on its alignment and parent layout.
 *
 * @param c Pointer to the button component.
 * @param parent_x X coordinate of the parent layout.
 * @param parent_y Y coordinate of the parent layout.
 * @param parent_width Width of the parent layout.
 * @param parent_height Height of the parent layout.
 */
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

/**
 * @brief Create a new button component.
 *
 * Allocates and initializes a new button with the given parameters.
 *
 * @param id String identifier for the button.
 * @param text Button label text.
 * @param align Alignment of the button within its parent layout.
 * @param text_color Color of the button text.
 * @param bg_color Background color of the button.
 * @return Pointer to the new button component. Exits on allocation failure.
 * @note Caller is responsible for freeing the component (if a free function exists).
 */
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
