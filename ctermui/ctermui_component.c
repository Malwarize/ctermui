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
    ctermui_pencil_solid_background(s->buffer, c->absolute_x, c->absolute_y, frame_width, frame_height, button->bg_color);
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
void ctermui_component_draw_solid_background(ctermui_screen_t s, ctermui_component c)
{
    if (c->type != SOLID_BACKGROUND)
    {
        fprintf(stderr, "ctermui_pencil_draw_background: invalid component type\n");
        exit(EXIT_FAILURE);
    }
    // TODO: dimension Validation
    SolidBackground *background = (SolidBackground *)c->core_component;
    for (int y = c->absolute_y; y < c->absolute_y + c->absolute_height; ++y)
    {
        for (int x = c->absolute_x; x < c->absolute_x + c->absolute_width; ++x)
        {
            ctermui_pencil_draw_char(s->buffer, x, y, s->buffer[x][y][0], s->buffer[x][y][1], background->color);
        }
    }
    return;
}
void ctermui_component_draw_soft_background(ctermui_screen_t s, ctermui_component c)
{
    if (c->type != SOFT_BACKGROUND)
    {
        fprintf(stderr, "ctermui_pencil_draw_background: invalid component type\n");
        exit(EXIT_FAILURE);
    }
    // TODO: dimension Validation
    SoftBackground *background = (SoftBackground *)c->core_component;
    ctermui_pencil_bucket(s->buffer, c->absolute_x, c->absolute_y, c->absolute_width, c->absolute_height, background->color);
}


void ctermui_button_calculate_absolute_position(ctermui_component c, int parent_x, int parent_y, int parent_width, int parent_height){
    Button* button = (Button*)c->core_component;
    int text_width = strlen(button->text);
    int frame_width = text_width + 2;
    int frame_height = 3;
    switch (button->align)
    {
        case CTERMUI_ALIGN_CENTER:
            c->absolute_x = parent_x + (parent_width - frame_width)/2;
            c->absolute_y = parent_y + (parent_height - frame_height)/2;
            break;
        case CTERMUI_ALIGN_LEFT_TOP:
            c->absolute_x = parent_x;
            c->absolute_y = parent_y;
            break;
        case CTERMUI_ALIGN_RIGHT_TOP:
            c->absolute_x = parent_x + parent_width - frame_width;
            c->absolute_y = parent_y;
            break;
        case CTERMUI_ALIGN_LEFT_BOTTOM:
            c->absolute_x = parent_x;
            c->absolute_y = parent_y + parent_height - frame_height;
            break;
        case CTERMUI_ALIGN_RIGHT_BOTTOM:
            c->absolute_x = parent_x + parent_width - frame_width;
            c->absolute_y = parent_y + parent_height - frame_height;
            break;
        case CTERMUI_ALIGN_TOP:
            c->absolute_x = parent_x + (parent_width - frame_width)/2;
            c->absolute_y = parent_y;
            break;
        case CTERMUI_ALIGN_BOTTOM:
            c->absolute_x = parent_x + (parent_width - frame_width)/2;
            c->absolute_y = parent_y + parent_height - frame_height;
            break;
        case CTERMUI_ALIGN_TOP_CENTER:
            c->absolute_x = parent_x;
            c->absolute_y = parent_y + (parent_height - frame_height)/2;
            break;
        case CTERMUI_ALIGN_BOTTOM_CENTER:
            c->absolute_x = parent_x + parent_width - frame_width;
            c->absolute_y = parent_y + (parent_height - frame_height)/2;
            break;
        case CTERMUI_ALIGN_LEFT_CENTER:
            c->absolute_x = parent_x;
            c->absolute_y = parent_y + (parent_height - frame_height)/2;
            break;
        case CTERMUI_ALIGN_RIGHT_CENTER:
            c->absolute_x = parent_x + parent_width - frame_width;
            c->absolute_y = parent_y + (parent_height - frame_height)/2;
            break;
        default:
            break;
    }
    c->absolute_width = frame_width;
    c->absolute_height = frame_height;
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
    c->calculate_absolute_position = ctermui_button_calculate_absolute_position;
    return c;
}

void  ctermui_text_calculate_absolute_position(ctermui_component c, int parent_x, int parent_y, int parent_width, int parent_height){
    Text* text = (Text*)c->core_component;
    int text_width = strlen(text->text);
    int text_height = 1;
    switch (text->align)
    {
        case CTERMUI_ALIGN_CENTER:
            c->absolute_x = parent_x + (parent_width - text_width)/2;
            c->absolute_y = parent_y + (parent_height - text_height)/2;
            break;
        case CTERMUI_ALIGN_LEFT_TOP:
            c->absolute_x = parent_x;
            c->absolute_y = parent_y;
            break;
        case CTERMUI_ALIGN_RIGHT_TOP:
            c->absolute_x = parent_x + parent_width - text_width;
            c->absolute_y = parent_y;
            break;
        case CTERMUI_ALIGN_LEFT_BOTTOM:
            c->absolute_x = parent_x;
            c->absolute_y = parent_y + parent_height - text_height;
            break;
        case CTERMUI_ALIGN_RIGHT_BOTTOM:
            c->absolute_x = parent_x + parent_width - text_width;
            c->absolute_y = parent_y + parent_height - text_height;
            break;
        case CTERMUI_ALIGN_TOP:
            c->absolute_x = parent_x + (parent_width - text_width)/2;
            c->absolute_y = parent_y;
            break;
        case CTERMUI_ALIGN_BOTTOM:
            c->absolute_x = parent_x + (parent_width - text_width)/2;
            c->absolute_y = parent_y + parent_height - text_height;
            break;
        case CTERMUI_ALIGN_TOP_CENTER:
            c->absolute_x = parent_x;
            c->absolute_y = parent_y + (parent_height - text_height)/2;
            break;
        case CTERMUI_ALIGN_BOTTOM_CENTER:
            c->absolute_x = parent_x + parent_width - text_width;
            c->absolute_y = parent_y + (parent_height - text_height)/2;
            break;
        case CTERMUI_ALIGN_LEFT_CENTER:
            c->absolute_x = parent_x;
            c->absolute_y = parent_y + (parent_height - text_height)/2;
            break;
        case CTERMUI_ALIGN_RIGHT_CENTER:
            c->absolute_x = parent_x + parent_width - text_width;
            c->absolute_y = parent_y + (parent_height - text_height)/2;
        default:
            break;
    }
    c->absolute_width = text_width;
    c->absolute_height = text_height;
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
    c->calculate_absolute_position = ctermui_text_calculate_absolute_position;
    return c;
}

void ctermui_calculate_absolute_position(ctermui_component c, int parent_x, int parent_y, int parent_width, int parent_height){
    Frame* frame = (Frame*)c->core_component;
    c->absolute_x = parent_x;
    c->absolute_y = parent_y;
    c->absolute_width = parent_width - 1; // -1 for border
    c->absolute_height = parent_height -1; // -1 for border
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
    c->draw = ctermui_component_draw_frame;
    c->calculate_absolute_position = ctermui_calculate_absolute_position;
    return c;
}

void ctermui_solid_background_calculate_absolute_position(ctermui_component c, int parent_x, int parent_y, int parent_width, int parent_height){
    SolidBackground* background = (SolidBackground*)c->core_component;
    c->absolute_x = parent_x;
    c->absolute_y = parent_y;
    c->absolute_width = parent_width;
    c->absolute_height = parent_height;
}

ctermui_component ctermui_new_solid_background(char* id,int color,int width, int height) {
    ctermui_component c = malloc(sizeof(struct ctermui_component));
    if(c==NULL){
        fprintf(stderr, "Error: could not allocate memory for background component\n");
    }
    SolidBackground* background_component = malloc(sizeof(SolidBackground));

    if(background_component == NULL){
        fprintf(stderr, "Error: could not allocate memory for background component\n");
    }

    background_component->color = color;
    c->core_component = background_component;
    c->type = SOLID_BACKGROUND;
    c->width = width;
    c->height = height;
    strcpy(c->id, id);
    c->draw = ctermui_component_draw_solid_background;
    c->calculate_absolute_position = ctermui_solid_background_calculate_absolute_position;
    return c;
}


void ctermui_progress_bar_calculate_absolute_position(ctermui_component c, int parent_x, int parent_y, int parent_width, int parent_height)
{
    ProgressBar* progress_bar = (ProgressBar*)c->core_component;
    c->absolute_x = parent_x;
    c->absolute_y = parent_y;
    c->absolute_width = parent_width;
    c->absolute_height = parent_height;
}

void ctermui_component_draw_progress_bar(ctermui_screen_t s, ctermui_component c){
    if (c->type != PROGRESS_BAR)
    {
        fprintf(stderr, "ctermui_pencil_draw_progress_bar: invalid component type\n");
        exit(EXIT_FAILURE);
    }
    // TODO: dimension Validation
    ProgressBar *progress_bar = (ProgressBar *)c->core_component;
    ctermui_pencil_solid_background(s->buffer, c->absolute_x, c->absolute_y, c->absolute_width, c->absolute_height, progress_bar->bg_color);
    int progress_width = (int)((float)progress_bar->progress / (float)progress_bar->max * (float)c->absolute_width);
    ctermui_pencil_solid_background(s->buffer, c->absolute_x, c->absolute_y, progress_width, c->absolute_height, progress_bar->bar_color);
    int text_width = strlen(progress_bar->text);
    int text_x = c->absolute_x + (c->absolute_width - text_width) / 2;
    int text_y = c->absolute_y + (c->absolute_height - 1) / 2;
    for(int i = 0; i < text_width; ++i){
        if(i + text_x < progress_width){
            ctermui_pencil_draw_char(s->buffer, text_x + i, text_y, progress_bar->text[i], progress_bar->text_color, progress_bar->bar_color);
        }else{
            ctermui_pencil_draw_char(s->buffer, text_x + i, text_y, progress_bar->text[i], progress_bar->text_color, progress_bar->bg_color);
        }
    }
}

ctermui_component ctermui_new_progress_bar(char* id, int bar_color, int bg_color, int max, int progress, char* text, int text_color) {
    ctermui_component c = malloc(sizeof(struct ctermui_component));
    if(c==NULL){
        fprintf(stderr, "Error: could not allocate memory for progress bar component\n");
    }
    ProgressBar* progress_bar_component = malloc(sizeof(ProgressBar));

    if(progress_bar_component == NULL){
        fprintf(stderr, "Error: could not allocate memory for progress bar component\n");
    }

    progress_bar_component->symbol =  PROGRESS_BAR_CHAR[0];
    progress_bar_component->bar_color = bar_color;
    progress_bar_component->bg_color = bg_color;
    progress_bar_component->max = max;
    progress_bar_component->progress = progress;
    progress_bar_component->text = text;
    progress_bar_component->text_color = text_color;
    
    c->core_component = progress_bar_component;
    c->type = PROGRESS_BAR;
    c->width = max;
    c->height = 1;
    strcpy(c->id, id);
    c->draw = ctermui_component_draw_progress_bar;
    c->calculate_absolute_position = ctermui_progress_bar_calculate_absolute_position;
    return c;
}

void ctermui_soft_background_calculate_absolute_position(ctermui_component c, int parent_x, int parent_y, int parent_width, int parent_height){
    c->absolute_x = parent_x;
    c->absolute_y = parent_y;
    c->absolute_width = parent_width;
    c->absolute_height = parent_height;
}
ctermui_component ctermui_new_soft_background(char* id,int color,int width, int height) 
{
    ctermui_component c = malloc(sizeof(struct ctermui_component));
    if(c==NULL){
        fprintf(stderr, "Error: could not allocate memory for background component\n");
    }
    SoftBackground* background_component = malloc(sizeof(SoftBackground));

    if(background_component == NULL){
        fprintf(stderr, "Error: could not allocate memory for background component\n");
    }

    background_component->color = color;
    c->core_component = background_component;
    c->type = SOFT_BACKGROUND;
    strcpy(c->id, id);
    c->draw = ctermui_component_draw_soft_background;
    c->calculate_absolute_position = ctermui_soft_background_calculate_absolute_position;
    return c;
}

ctermui_component ctermui_new_custom_component(
    char* id,
    void (*draw)(ctermui_screen_t s,ctermui_component c),
    void (*calculate_absolute_position)(
        ctermui_component c,
        int parent_x,
        int parent_y,
        int parent_width,
        int parent_height
)
){
    ctermui_component c = (ctermui_component)malloc(sizeof(struct ctermui_component));
    if(!c){
        fprintf(stderr, "Error: could not allocate memory for component\n");
        exit(EXIT_FAILURE);
    }
    c->type = CUSTOM;
    strcpy(c->id, id);
    c->draw = draw;
    c->calculate_absolute_position = calculate_absolute_position;
    return c;
}
