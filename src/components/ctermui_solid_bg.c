#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"

void ctermui_component_draw_solid_background(ctermui_screen_t s,
                                             ctermui_component_t c) {
    // TODO: dimension Validation
    SolidBackground *background = c->core_component;
    for (size_t y = c->y; y < c->y + c->height; ++y) {
        for (size_t x = c->x; x < c->x + c->width; ++x) {
            ctermui_pencil_draw_char(s->buffer, x, y, s->buffer[x][y]->character,
                                     s->buffer[x][y]->foreground_color,
                                     background->color, 0);
        }
    }
}

void ctermui_solid_background_calculate_absolute_position(
    ctermui_component_t c, size_t parent_x, size_t parent_y,
    size_t parent_width, size_t parent_height) {
    c->x = parent_x;
    c->y = parent_y;
    c->width = parent_width;
    c->height = parent_height;
}

ctermui_component_t ctermui_new_solid_background(char *id, int8_t color,
size_t width, size_t height) {
    SolidBackground *background_component = malloc(sizeof(SolidBackground));
    if (background_component == NULL) {
        fprintf(stderr,
                "Error: could not allocate memory for background component\n");
        exit(EXIT_FAILURE);
    }

    background_component->color = color;
    return ctermui_new_custom_component(id, background_component, ctermui_component_draw_solid_background, ctermui_solid_background_calculate_absolute_position);
}
