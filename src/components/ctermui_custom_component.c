#include "ctermui_component.h"
#include "ctermui_screen.h"

// what is Component?
// is struct that have 2 attrs (x,y) to indicate where the pencil will start
// drawing and it have width and height to indicate the size of the component
// component must have 2 method
// draw() to draw the component
// calculate_absolute_position(parent_x, parent_y, parent_width, parent_height)
// to calculate the position of the component whatever you like and it have
// core_component to store the data of the custom component like core_component
// can be Button, Text, Frame, SolidBackground, SoftBackground, ProgressBar,
// TextInput, Barchart, ScatterPlot

// to make custom component you need to make a struct that have the data of the
// component this struct will be the core_component and you need to make
// ctermui_component_draw_button(ctermui_screen_t s, ctermui_component_t c) and
// ctermui_button_calculate_absolute_position(ctermui_component_t c, size_t
// parent_x, size_t parent_y, size_t parent_width, size_t parent_height) make
// constructor for the custom component ctermui_component_t the constructor must
// call ctermui_new_custom_component(id, ctermui_component_draw_button,
// ctermui_button_calculate_absolute_position, button_component)

ctermui_component_t ctermui_new_custom_component(
    char *id, void *core_component,
    void (*draw)(ctermui_screen_t s, ctermui_component_t c),
    void (*calculate_absolute_position)(ctermui_component_t c, size_t parent_x,
                                        size_t parent_y, size_t parent_width,
                                        size_t parent_height)) {
  ctermui_component_t c = malloc(sizeof(struct ctermui_component));
  if (!c) {
    fprintf(stderr, "Error: could not allocate memory for component\n");
    exit(EXIT_FAILURE);
  }
  c->core_component = core_component;
  c->type = CUSTOM;
  strcpy(c->id, id);
  c->draw = draw;
  c->calculate_absolute_position = calculate_absolute_position;
  c->core_component = core_component;
  return c;
}

