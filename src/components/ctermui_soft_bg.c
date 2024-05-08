#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"
void ctermui_component_draw_soft_background(ctermui_screen_t s,
                                            ctermui_component_t c) {
  // TODO: dimension Validation
  SoftBackground *background = (SoftBackground *)c->core_component;
  ctermui_pencil_bucket(s->buffer, c->x, c->y, c->width, c->height,
                        background->color);
}

void ctermui_soft_background_calculate_absolute_position(ctermui_component_t c,
                                                         size_t parent_x,
                                                         size_t parent_y,
                                                         size_t parent_width,
                                                         size_t parent_height) {
  c->x = parent_x;
  c->y = parent_y;
  c->width = parent_width;
  c->height = parent_height;
}

ctermui_component_t ctermui_new_soft_background(char *id, int8_t color) {
  SoftBackground *background_component = malloc(sizeof(SoftBackground));

  if (background_component == NULL) {
    fprintf(stderr,
            "Error: could not allocate memory for background component\n");
    exit(EXIT_FAILURE);
  }
  background_component->color = color;
  return ctermui_new_custom_component(
      id, background_component, ctermui_component_draw_soft_background,
      ctermui_soft_background_calculate_absolute_position);
}
