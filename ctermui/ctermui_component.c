#include "ctermui_component.h"

ctermui_component ctermui_new_button(char* text, void (*on_click)(void)) {
    ctermui_component ctermui_component = malloc(sizeof(struct ctermui_component));
    ctermui_component->type = BUTTON;
    Button* button_component = malloc(sizeof(Button));
    strcpy(button_component->text, text);
    button_component->on_click = on_click;
    ctermui_component->core_component = button_component;
    return ctermui_component;
}


ctermui_component ctermui_new_text(char* text) {
    ctermui_component c = malloc(sizeof(struct ctermui_component));
    c->type = TEXT;
    Text* text_component = malloc(sizeof(Text));
    strcpy(text_component->text, text);
    c->core_component = text_component;
    return c;
}

ctermui_component ctermui_component_new() {
    ctermui_component c;
    c=(ctermui_component)malloc(sizeof(struct ctermui_component));
    return c;
}