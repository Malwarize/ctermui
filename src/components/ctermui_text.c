#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"


void ctermui_text_calculate_absolute_position(ctermui_component_t c,
                                              size_t parent_x, size_t parent_y,
                                              size_t parent_width,
                                              size_t parent_height) {
    Text *text = (Text *)c->core_component;
    size_t text_width = strlen(text->text);
    size_t text_height = 1;
    switch (text->align) {
        case CTERMUI_ALIGN_CENTER:
            c->x = parent_x + (parent_width - text_width) / 2;
        c->y = parent_y + (parent_height - text_height) / 2;
        break;
        case CTERMUI_ALIGN_LEFT_TOP:
            c->x = parent_x;
        c->y = parent_y;
        break;
        case CTERMUI_ALIGN_RIGHT_TOP:
            c->x = parent_x + parent_width - text_width;
        c->y = parent_y;
        break;
        case CTERMUI_ALIGN_LEFT_BOTTOM:
            c->x = parent_x;
        c->y = parent_y + parent_height - text_height;
        break;
        case CTERMUI_ALIGN_RIGHT_BOTTOM:
            c->x = parent_x + parent_width - text_width;
        c->y = parent_y + parent_height - text_height;
        break;
        case CTERMUI_ALIGN_TOP:
            c->x = parent_x + (parent_width - text_width) / 2;
        c->y = parent_y;
        break;
        case CTERMUI_ALIGN_BOTTOM:
            c->x = parent_x + (parent_width - text_width) / 2;
        c->y = parent_y + parent_height - text_height;
        break;
        case CTERMUI_ALIGN_TOP_CENTER:
            c->x = parent_x;
        c->y = parent_y + (parent_height - text_height) / 2;
        break;
        case CTERMUI_ALIGN_BOTTOM_CENTER:
            c->x = parent_x + parent_width - text_width;
        c->y = parent_y + (parent_height - text_height) / 2;
        break;
        case CTERMUI_ALIGN_LEFT_CENTER:
            c->x = parent_x;
        c->y = parent_y + (parent_height - text_height) / 2;
        break;
        case CTERMUI_ALIGN_RIGHT_CENTER:
            c->x = parent_x + parent_width - text_width;
        c->y = parent_y + (parent_height - text_height) / 2;
        default:
            break;
    }
    c->width = text_width;
    c->height = text_height;
}

ctermui_component_t ctermui_new_text(char *id, char *text, int8_t color,
int8_t bg_color, size_t align) {

    Text *text_component = malloc(sizeof(Text));
    if (text_component == NULL) {
        fprintf(stderr, "Error: could not allocate memory for text component\n");
        exit(EXIT_FAILURE);
    }
    strcpy(text_component->text, text);
    text_component->color = color;
    text_component->bg_color = bg_color;
    text_component->align = align;
    return ctermui_new_custom_component(id, text_component, ctermui_component_draw_label, ctermui_text_calculate_absolute_position);
}