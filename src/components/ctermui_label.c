#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"


void ctermui_component_draw_label(ctermui_screen_t s, ctermui_component_t c) {
    // TODO: dimension Validation
    Text *text = (Text *)c->core_component;
    ctermui_pencil_draw_text(s->buffer, c->x, c->y, text->text,
                             text->color, text->bg_color);
}

