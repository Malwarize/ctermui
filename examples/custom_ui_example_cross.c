#include "ctermui/ctermui_screen.h"
#include "ctermui/ctermui_pencil.h"

// to make a custom component, you need to define a struct
typedef struct CustomComponentCross {
    int color;
} CustomComponentCross;


// make a function to draw the component

void draw_custom_component(ctermui_screen_t s, ctermui_component_t c) {
    CustomComponentCross *cross = (CustomComponentCross *) c->core_component; // cast this fucking bastard
    size_t vertical_starting_point = c->absolute_x + c->absolute_width / 2;
    ctermui_pencil_draw_line(
            s->buffer,
            CTERMUI_VERTICAL,
            vertical_starting_point,
            c->absolute_y,
            c->absolute_height,
            CTERMUI_VERTICAL_LINE,
            cross->color,
            cross->color
                            );

    size_t horizontal_starting_point = c->absolute_y + c->absolute_height / 2;
    ctermui_pencil_draw_line(
            s->buffer,
            CTERMUI_HORIZONTAL,
            c->absolute_x,
            horizontal_starting_point,
            c->absolute_width,
            CTERMUI_VERTICAL_LINE,
            cross->color,
            cross->color
                            );
}

// make a function that calculates the absolute position of the component
void calculate_abs_position_custom_component(
        ctermui_component_t c,
        size_t parent_x,
        size_t parent_y,
        size_t parent_width,
        size_t parent_height
                                            ) {
    c->absolute_x = parent_x;
    c->absolute_y = parent_y;
    c->absolute_width = parent_width;
    c->absolute_height = parent_height;
}

int main() {
    ctermui_screen_t screen = ctermui_screen_init();
    ctermui_widget_t root = ctermui_widget_new_root(
            CTERMUI_HORIZONTAL, screen->width, screen->height
                                                   );


    CustomComponentCross cross = {
            .color = CTERMUI_RED
    };
    ctermui_component_t custom_component = ctermui_new_custom_component(
            "custom_component", draw_custom_component, calculate_abs_position_custom_component, (void *) &cross
                                                                       );
    ctermui_widget_add_component(root, custom_component);
    ctermui_screen_set_widget_root(screen, root);
    ctermui_screen_loop_start(screen, NULL, 0);
    return 0;
}