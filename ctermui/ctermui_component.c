#include "ctermui_component.h"
#include "ctermui_screen.h"
void ctermui_component_draw_button(ctermui_screen_t s,  ctermui_component c)
{
    if (c->type != BUTTON)
    {
        fprintf(stderr, "ctermui_pencil_draw_button: invalid component type\n");
        exit(EXIT_FAILURE);
    }
    // TODO: dimension Validation
    Button *button = (Button *)c->core_component;
    int text_width = strlen(button->text);
    int frame_width = text_width + 2;
    int frame_height = 3;
    ctermui_pencil_colorify_background(s->buffer, c->absolute_x, c->absolute_y, frame_width, frame_height, button->bg_color);
    ctermui_pencil_draw_text(s->buffer, c->absolute_x + (frame_width - text_width) / 2, c->absolute_y + (frame_height - 1) / 2, button->text, button->text_color, button->bg_color);
    return;
}
void ctermui_component_draw_label(ctermui_screen_t s,  ctermui_component c)
{
    if (c->type != TEXT)
    {
        fprintf(stderr, "ctermui_pencil_draw_label: invalid component type\n");
        exit(EXIT_FAILURE);
    }
    // TODO: dimension Validation
    Text *text = (Text *)c->core_component;
    ctermui_pencil_draw_text(s->buffer, c->absolute_x, c->absolute_y, text->text, text->color, text->bg_color);
    return;
}
void ctermui_component_draw_frame(ctermui_screen_t s, ctermui_component c)
{
    if (c->type != FRAME)
    {
        fprintf(stderr, "ctermui_pencil_draw_frame: invalid component type\n");
        exit(EXIT_FAILURE);
    }
    // TODO: dimension Validation
    Frame *frame = (Frame *)c->core_component;
    ctermui_pencil_draw_rect(s->buffer, c->absolute_x, c->absolute_y, c->absolute_width, c->absolute_height, frame->color, frame->bg_color);
    return;
}
void ctermui_component_draw_background(ctermui_screen_t s, ctermui_component c)
{
    if (c->type != BACKGROUND)
    {
        fprintf(stderr, "ctermui_pencil_draw_background: invalid component type\n");
        exit(EXIT_FAILURE);
    }
    // TODO: dimension Validation
    Background *background = (Background *)c->core_component;
    for (int y = c->absolute_y; y < c->absolute_y + c->absolute_height; ++y)
    {
        for (int x = c->absolute_x; x < c->absolute_x + c->absolute_width; ++x)
        {
            ctermui_pencil_draw_char(s->buffer, x, y, s->buffer[x][y][0], s->buffer[x][y][1], background->color);
        }
    }
    return;
}

ctermui_component ctermui_new_button(char* id, char* text,int align, int text_color, int bg_color) {
    ctermui_component c = malloc(sizeof(struct ctermui_component));
    c->type = BUTTON;
    Button* button_component = malloc(sizeof(Button));
    if(button_component == NULL){
        fprintf(stderr, "Error: could not allocate memory for button component\n");
    }
    strcpy(button_component->text, text);
    button_component->align = align;
    button_component->text_color = text_color;
    button_component->bg_color = bg_color;
    c->core_component = button_component;
    c->width = strlen(text);
    c->height = 1;
    strcpy(c->id, id);
    c->draw = ctermui_component_draw_button;
    return c;
}
ctermui_component ctermui_new_text(char* id,char* text,int color, int bg_color, int align) {
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
    strcpy(c->id, id);
    c->draw = ctermui_component_draw_label;
    return c;
}
ctermui_component ctermui_new_frame(char* id,int color, int bg_color) {
    ctermui_component c = malloc(sizeof(struct ctermui_component));
    c->type = FRAME;
    Frame* frame_component = malloc(sizeof(Frame));
    if(frame_component == NULL){
        fprintf(stderr, "Error: could not allocate memory for frame component\n");
    }
    frame_component->color = color;
    frame_component->bg_color = bg_color;
    c->core_component = frame_component;
    c->width = 1;
    c->draw = ctermui_component_draw_frame;
    return c;
}
ctermui_component ctermui_new_background(char* id,int color,int width, int height) {

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
    strcpy(c->id, id);
    c->draw = ctermui_component_draw_background;
    return c;
}