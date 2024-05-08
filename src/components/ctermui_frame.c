#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"
#include <string.h>

void ctermui_component_draw_frame(ctermui_screen_t s, ctermui_component_t c) {
    // TODO: dimension Validation
    Frame *frame = (Frame *)c->core_component;
    ctermui_pencil_draw_rect(s->buffer, c->x, c->y,
                             c->width, c->height, frame->color,
                             frame->bg_color);
}

void ctermui_frame_calculate_absolute_position(ctermui_component_t c, size_t parent_x,
                                         size_t parent_y, size_t parent_width,
                                         size_t parent_height) {
    c->x = parent_x;
    c->y = parent_y;
    c->width = parent_width - 1;   // -1 for border
    c->height = parent_height - 1; // -1 for border
}

ctermui_component_t ctermui_new_frame(char *id, int8_t color, int8_t bg_color) {
    Frame *frame_component = malloc(sizeof(Frame));
    if (frame_component == NULL) {
        fprintf(stderr, "Error: could not allocate memory for frame component\n");
        exit(EXIT_FAILURE);
    }
    frame_component->color = color;
    frame_component->bg_color = bg_color;
    return ctermui_new_custom_component(id, frame_component, ctermui_component_draw_frame, ctermui_frame_calculate_absolute_position);
}