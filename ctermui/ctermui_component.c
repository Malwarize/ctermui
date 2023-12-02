#include "ctermui_component.h"

ctermui_component ctermui_new_button(char* text, void (*on_click)(void)) {
    ctermui_component ctermui_component = malloc(sizeof(struct ctermui_component));
    ctermui_component->type = BUTTON;
    Button* button_component = malloc(sizeof(Button));
    if(button_component == NULL){
        fprintf(stderr, "Error: could not allocate memory for button component\n");
    }
    strcpy(button_component->text, text);
    button_component->on_click = on_click;
    ctermui_component->core_component = button_component;
    return ctermui_component;
}


ctermui_component ctermui_new_text(char* text,int color, int bg_color, int align) {
    ctermui_component c = malloc(sizeof(struct ctermui_component));
    c->type = TEXT;
    Text* text_component = malloc(sizeof(Text));
    if(text_component == NULL){
        fprintf(stderr, "Error: could not allocate memory for text component\n");
    }
    strcpy(text_component->text, text);
    text_component->color = color;
    text_component->bg_color = bg_color;
    text_component->align = align;
    c->core_component = text_component;
    c->width = strlen(text);
    c->height = 1;
    return c;
}

// Frame component
ctermui_component ctermui_new_frame(int color, int bg_color) {
    ctermui_component c = malloc(sizeof(struct ctermui_component));
    c->type = FRAME;
    Frame* frame_component = malloc(sizeof(Frame));
    if(frame_component == NULL){
        fprintf(stderr, "Error: could not allocate memory for frame component\n");
    }
    frame_component->color = color;
    frame_component->bg_color = bg_color;
    c->core_component = frame_component;
    return c;
}

ctermui_component ctermui_new_background(int color,int width, int height) {
    ctermui_component c = malloc(sizeof(struct ctermui_component));
    if(c==NULL){
        fprintf(stderr, "Error: could not allocate memory for background component\n");
    }
    Background* background_component = malloc(sizeof(Background));

    if(background_component == NULL){
        fprintf(stderr, "Error: could not allocate memory for background component\n");
    }

    background_component->color = color;
    c->core_component = background_component;
    c->type = BACKGROUND;


    c->width = width;
    c->height = height;
    return c;
}

ctermui_component ctermui_component_new() {
    ctermui_component c;
    c=(ctermui_component)malloc(sizeof(struct ctermui_component));
    if(c==NULL){
        fprintf(stderr, "Error: could not allocate memory for component\n");
    }
    return c;
}