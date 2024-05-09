#include "ctermui_pencil.h"
#include "ctermui_screen.h"

// to make a custom component, you need to define a struct
typedef struct CustomComponentCross {
  int color;
} CustomComponentCross;

// make a function to draw the component

void draw_custom_component(ctermui_screen_t s, ctermui_component_t c) {
  CustomComponentCross *cross =
      (CustomComponentCross *)c->core_component; // cast this fucking bastard
  size_t vertical_starting_point = c->x + c->width / 2;
  ctermui_pencil_draw_line(s->buffer, CTERMUI_VERTICAL, vertical_starting_point,
                           c->y, c->height, CTERMUI_VERTICAL_LINE, cross->color,
                           cross->color);

  size_t horizontal_starting_point = c->y + c->height / 2;
  ctermui_pencil_draw_line(s->buffer, CTERMUI_HORIZONTAL, c->x,
                           horizontal_starting_point, c->width,
                           CTERMUI_VERTICAL_LINE, cross->color, cross->color);
}

// make a function that calculates the absolute position of the component
void calculate_abs_position_custom_component(ctermui_component_t c,
                                             size_t parent_x, size_t parent_y,
                                             size_t parent_width,
                                             size_t parent_height) {
  c->x = parent_x;
  c->y = parent_y;
  c->width = parent_width;
  c->height = parent_height;
}

int main() {
  ctermui_screen_t screen = ctermui_screen_init();
  ctermui_layout_t root = ctermui_layout_new_root(
      CTERMUI_HORIZONTAL, screen->width, screen->height);

  CustomComponentCross cross = {.color = CTERMUI_RED};
  ctermui_component_t custom_component = ctermui_new_custom_component(
      "custom_component", (void *)&cross, draw_custom_component,
      calculate_abs_position_custom_component);
  ctermui_layout_add_component(root, custom_component);
  ctermui_screen_set_layout_root(screen, root);
  ctermui_screen_loop_start(screen, NULL, 1000);
  return 0;
}
