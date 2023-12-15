#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"
void ctermui_component_draw_button(ctermui_screen_t s,
                                   ctermui_component c)
{
  if (c->type != BUTTON) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_button: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  // TODO: dimension Validation
  Button* button = (Button*)c->core_component;
  size_t text_width = strlen(button->text);
  size_t frame_width = text_width + 2;
  size_t frame_height = 3;
  ctermui_pencil_solid_background(s->buffer,
                                  c->absolute_x,
                                  c->absolute_y,
                                  frame_width,
                                  frame_height,
                                  button->bg_color);
  ctermui_pencil_draw_text(
    s->buffer,
    c->absolute_x + (frame_width - text_width) / 2,
    c->absolute_y + (frame_height - 1) / 2,
    button->text,
    button->text_color,
    button->bg_color);
  ;
}
void ctermui_component_draw_label(ctermui_screen_t s,
                                  ctermui_component c)
{
  if (c->type != TEXT) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_label: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  // TODO: dimension Validation
  Text* text = (Text*)c->core_component;
  ctermui_pencil_draw_text(s->buffer,
                           c->absolute_x,
                           c->absolute_y,
                           text->text,
                           text->color,
                           text->bg_color);
}
void ctermui_component_draw_frame(ctermui_screen_t s,
                                  ctermui_component c)
{
  if (c->type != FRAME) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_frame: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  // TODO: dimension Validation
  Frame* frame = (Frame*)c->core_component;
  ctermui_pencil_draw_rect(s->buffer,
                           c->absolute_x,
                           c->absolute_y,
                           c->absolute_width,
                           c->absolute_height,
                           frame->color,
                           frame->bg_color);
}
void ctermui_component_draw_solid_background(
  ctermui_screen_t s, ctermui_component c)
{
  if (c->type != SOLID_BACKGROUND) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_background: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  // TODO: dimension Validation
  SolidBackground* background =
    (SolidBackground*)c->core_component;
  for (size_t y = c->absolute_y;
       y < c->absolute_y + c->absolute_height;
       ++y) {
    for (size_t x = c->absolute_x;
         x < c->absolute_x + c->absolute_width;
         ++x) {
      ctermui_pencil_draw_char(s->buffer,
                               x,
                               y,
                               s->buffer[x][y][0],
                               s->buffer[x][y][1],
                               background->color);
    }
  }
}
void ctermui_component_draw_soft_background(
  ctermui_screen_t s, ctermui_component c)
{
  if (c->type != SOFT_BACKGROUND) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_background: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  // TODO: dimension Validation
  SoftBackground* background =
    (SoftBackground*)c->core_component;
  ctermui_pencil_bucket(s->buffer,
                        c->absolute_x,
                        c->absolute_y,
                        c->absolute_width,
                        c->absolute_height,
                        background->color);
}


void ctermui_button_calculate_absolute_position(
  ctermui_component c,
  size_t parent_x,
  size_t parent_y,
  size_t parent_width,
  size_t parent_height)
{
  Button* button = (Button*)c->core_component;
  size_t text_width = strlen(button->text);
  size_t frame_width = text_width + 2;
  size_t frame_height = 3;
  switch (button->align) {
    case CTERMUI_ALIGN_CENTER:
      c->absolute_x =
        parent_x + (parent_width - frame_width) / 2;
      c->absolute_y =
        parent_y + (parent_height - frame_height) / 2;
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
      c->absolute_y =
        parent_y + parent_height - frame_height;
      break;
    case CTERMUI_ALIGN_RIGHT_BOTTOM:
      c->absolute_x = parent_x + parent_width - frame_width;
      c->absolute_y =
        parent_y + parent_height - frame_height;
      break;
    case CTERMUI_ALIGN_TOP:
      c->absolute_x =
        parent_x + (parent_width - frame_width) / 2;
      c->absolute_y = parent_y;
      break;
    case CTERMUI_ALIGN_BOTTOM:
      c->absolute_x =
        parent_x + (parent_width - frame_width) / 2;
      c->absolute_y =
        parent_y + parent_height - frame_height;
      break;
    case CTERMUI_ALIGN_TOP_CENTER:
      c->absolute_x = parent_x;
      c->absolute_y =
        parent_y + (parent_height - frame_height) / 2;
      break;
    case CTERMUI_ALIGN_BOTTOM_CENTER:
      c->absolute_x = parent_x + parent_width - frame_width;
      c->absolute_y =
        parent_y + (parent_height - frame_height) / 2;
      break;
    case CTERMUI_ALIGN_LEFT_CENTER:
      c->absolute_x = parent_x;
      c->absolute_y =
        parent_y + (parent_height - frame_height) / 2;
      break;
    case CTERMUI_ALIGN_RIGHT_CENTER:
      c->absolute_x = parent_x + parent_width - frame_width;
      c->absolute_y =
        parent_y + (parent_height - frame_height) / 2;
      break;
    default:
      break;
  }
  c->absolute_width = frame_width;
  c->absolute_height = frame_height;
}

ctermui_component ctermui_new_button(char* id,
                                     char* text,
                                     size_t align,
                                     size_t text_color,
                                     size_t bg_color)
{
  ctermui_component c =
    malloc(sizeof(struct ctermui_component));
  c->type = BUTTON;
  Button* button_component = malloc(sizeof(Button));
  if (button_component == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for button component\n");
    exit(EXIT_FAILURE);
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
  c->calculate_absolute_position =
    ctermui_button_calculate_absolute_position;
  return c;
}

void ctermui_text_calculate_absolute_position(
  ctermui_component c,
  size_t parent_x,
  size_t parent_y,
  size_t parent_width,
  size_t parent_height)
{
  Text* text = (Text*)c->core_component;
  size_t text_width = strlen(text->text);
  size_t text_height = 1;
  switch (text->align) {
    case CTERMUI_ALIGN_CENTER:
      c->absolute_x =
        parent_x + (parent_width - text_width) / 2;
      c->absolute_y =
        parent_y + (parent_height - text_height) / 2;
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
      c->absolute_y =
        parent_y + parent_height - text_height;
      break;
    case CTERMUI_ALIGN_RIGHT_BOTTOM:
      c->absolute_x = parent_x + parent_width - text_width;
      c->absolute_y =
        parent_y + parent_height - text_height;
      break;
    case CTERMUI_ALIGN_TOP:
      c->absolute_x =
        parent_x + (parent_width - text_width) / 2;
      c->absolute_y = parent_y;
      break;
    case CTERMUI_ALIGN_BOTTOM:
      c->absolute_x =
        parent_x + (parent_width - text_width) / 2;
      c->absolute_y =
        parent_y + parent_height - text_height;
      break;
    case CTERMUI_ALIGN_TOP_CENTER:
      c->absolute_x = parent_x;
      c->absolute_y =
        parent_y + (parent_height - text_height) / 2;
      break;
    case CTERMUI_ALIGN_BOTTOM_CENTER:
      c->absolute_x = parent_x + parent_width - text_width;
      c->absolute_y =
        parent_y + (parent_height - text_height) / 2;
      break;
    case CTERMUI_ALIGN_LEFT_CENTER:
      c->absolute_x = parent_x;
      c->absolute_y =
        parent_y + (parent_height - text_height) / 2;
      break;
    case CTERMUI_ALIGN_RIGHT_CENTER:
      c->absolute_x = parent_x + parent_width - text_width;
      c->absolute_y =
        parent_y + (parent_height - text_height) / 2;
    default:
      break;
  }
  c->absolute_width = text_width;
  c->absolute_height = text_height;
}

ctermui_component ctermui_new_text(
  char* id, char* text, size_t color, size_t bg_color, size_t align)
{
  ctermui_component c =
    malloc(sizeof(struct ctermui_component));
  c->type = TEXT;
  Text* text_component = malloc(sizeof(Text));
  if (text_component == NULL) {
    fprintf(
    stderr,
    "Error: could not allocate memory for text component\n"
      );
      exit(EXIT_FAILURE);
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

void ctermui_calculate_absolute_position(
  ctermui_component c,
  size_t parent_x,
  size_t parent_y,
  size_t parent_width,
  size_t parent_height)
{
  c->absolute_x = parent_x;
  c->absolute_y = parent_y;
  c->absolute_width = parent_width - 1;    // -1 for border
  c->absolute_height = parent_height - 1;  // -1 for border
}

ctermui_component ctermui_new_frame(char* id,
                                    size_t color,
                                    size_t bg_color)
{
  ctermui_component c =
    malloc(sizeof(struct ctermui_component));
  c->type = FRAME;
  Frame* frame_component = malloc(sizeof(Frame));
  if (frame_component == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for frame component\n");
    exit(EXIT_FAILURE);
  }
  frame_component->color = color;
  frame_component->bg_color = bg_color;
  c->core_component = frame_component;
  c->draw = ctermui_component_draw_frame;
  c->calculate_absolute_position =
    ctermui_calculate_absolute_position;
  return c;
}

void ctermui_solid_background_calculate_absolute_position(
  ctermui_component c,
  size_t parent_x,
  size_t parent_y,
  size_t parent_width,
  size_t parent_height)
{
  c->absolute_x = parent_x;
  c->absolute_y = parent_y;
  c->absolute_width = parent_width;
  c->absolute_height = parent_height;
}

ctermui_component ctermui_new_solid_background(char* id,
                                               size_t color,
                                               size_t width,
                                               size_t height)
{
  ctermui_component c =
    malloc(sizeof(struct ctermui_component));
  if (c == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for background component\n");
    exit(EXIT_FAILURE);
  }
  SolidBackground* background_component =
    malloc(sizeof(SolidBackground));

  if (background_component == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for background component\n");
    exit(EXIT_FAILURE);
  }

  background_component->color = color;
  c->core_component = background_component;
  c->type = SOLID_BACKGROUND;
  c->width = width;
  c->height = height;
  strcpy(c->id, id);
  c->draw = ctermui_component_draw_solid_background;
  c->calculate_absolute_position =
    ctermui_solid_background_calculate_absolute_position;
  return c;
}

void ctermui_progress_bar_calculate_absolute_position(
  ctermui_component c,
  size_t parent_x,
  size_t parent_y,
  size_t parent_width,
  size_t parent_height)
{
  c->absolute_x = parent_x;
  c->absolute_y = parent_y;
  c->absolute_width = parent_width;
  c->absolute_height = parent_height;
}

void ctermui_component_draw_progress_bar(
  ctermui_screen_t s, ctermui_component c)
{
  if (c->type != PROGRESS_BAR) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_progress_bar: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  ProgressBar* progress_bar =
    (ProgressBar*)c->core_component;
  if (progress_bar->orientation == CTERMUI_HORIZONTAL) {
    ctermui_pencil_solid_background(s->buffer,
                                    c->absolute_x,
                                    c->absolute_y,
                                    c->absolute_width,
                                    c->absolute_height,
                                    progress_bar->bg_color);
    size_t progress_width =
      (size_t)((float)progress_bar->progress /
            (float)progress_bar->max *
            (float)c->absolute_width);
    ctermui_pencil_solid_background(
      s->buffer,
      c->absolute_x,
      c->absolute_y,
      progress_width,
      c->absolute_height,
      progress_bar->bar_color);
    size_t text_width = strlen(progress_bar->text);
    size_t text_x =
      c->absolute_x + (c->absolute_width - text_width) / 2;
    size_t text_y =
      c->absolute_y + (c->absolute_height - 1) / 2;
    for (size_t i = 0; i < text_width; ++i) {
      if (i + text_x < progress_width) {
        ctermui_pencil_draw_char(s->buffer,
                                 text_x + i,
                                 text_y,
                                 progress_bar->text[i],
                                 progress_bar->text_color,
                                 progress_bar->bar_color);
      }
      else {
        ctermui_pencil_draw_char(s->buffer,
                                 text_x + i,
                                 text_y,
                                 progress_bar->text[i],
                                 progress_bar->text_color,
                                 progress_bar->bg_color);
      }
    }
  }
  else {
    ctermui_pencil_solid_background(s->buffer,
                                    c->absolute_x,
                                    c->absolute_y,
                                    c->absolute_width,
                                    c->absolute_height,
                                    progress_bar->bg_color);
    size_t progress_height =
      (size_t)((float)progress_bar->progress /
            (float)progress_bar->max *
            (float)c->absolute_height);
    ctermui_pencil_solid_background(
      s->buffer,
      c->absolute_x,
      c->absolute_y + c->absolute_height - progress_height,
      c->absolute_width,
      progress_height,
      progress_bar->bar_color);
    size_t text_width = strlen(progress_bar->text);
    size_t text_x =
      c->absolute_x + (c->absolute_width - text_width) / 2;
    size_t text_y =
      c->absolute_y + (c->absolute_height - 1) / 2;
    for (size_t i = 0; i < text_width; ++i) {
      if (i + text_x < progress_height) {
        ctermui_pencil_draw_char(s->buffer,
                                 text_x + i,
                                 text_y,
                                 progress_bar->text[i],
                                 progress_bar->text_color,
                                 progress_bar->bar_color);
      }
      else {
        ctermui_pencil_draw_char(s->buffer,
                                 text_x + i,
                                 text_y,
                                 progress_bar->text[i],
                                 progress_bar->text_color,
                                 progress_bar->bg_color);
      }
    }
  }
}

ctermui_component ctermui_new_progress_bar(char* id,
                                           size_t bar_color,
                                           size_t bg_color,
                                           size_t max,
                                           size_t progress,
                                           char* text,
                                           size_t text_color,
                                           size_t orientation)
{
  ctermui_component c =
    malloc(sizeof(struct ctermui_component));
  if (c == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for progress bar component\n");
    exit(EXIT_FAILURE);
  }
  ProgressBar* progress_bar_component =
    malloc(sizeof(ProgressBar));

  if (progress_bar_component == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for progress bar component\n");
    exit(EXIT_FAILURE);
  }

  progress_bar_component->symbol = PROGRESS_BAR_CHAR[0];
  progress_bar_component->bar_color = bar_color;
  progress_bar_component->bg_color = bg_color;
  progress_bar_component->max = max;
  progress_bar_component->progress = progress;
  progress_bar_component->text = text;
  progress_bar_component->text_color = text_color;
  progress_bar_component->orientation = orientation;

  c->core_component = progress_bar_component;
  c->type = PROGRESS_BAR;
  c->width = max;
  c->height = 1;
  strcpy(c->id, id);
  c->draw = ctermui_component_draw_progress_bar;
  c->calculate_absolute_position =
    ctermui_progress_bar_calculate_absolute_position;
  return c;
}


void ctermui_text_input_draw(ctermui_screen_t s,
                             ctermui_component c)
{
  if (c->type != TEXT_INPUT) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_text_input: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  TextInput* text_input = (TextInput*)c->core_component;
  size_t text_width = strlen(text_input->text);
  size_t frame_width = c->absolute_width;
  size_t frame_height = c->absolute_height;
  if(text_width >= frame_width){
    frame_width = frame_width + (text_width-frame_width);
  }
  ctermui_pencil_solid_background(s->buffer,
                                  c->absolute_x,
                                  c->absolute_y,
                                  frame_width,
                                  frame_height,
                                  text_input->bg_color);
  ctermui_pencil_draw_text(
    s->buffer,
    c->absolute_x,
    c->absolute_y + (frame_height - 1) / 2,
    text_input->text,
    text_input->text_color,
    text_input->bg_color);

}


struct comp_key {
  ctermui_component c;
  size_t key;
};

void ctermui_text_input_write(void* args)
{
  struct comp_key* ck = (struct comp_key*)args;
  ctermui_component c = ck->c;
  size_t key = ck->key;
  if (c->type != TEXT_INPUT) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_text_input: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  TextInput* text_input = (TextInput*)c->core_component;
  if (text_input->selected != 1) {
    return;
  }
  size_t text_length = strlen(text_input->text);
  if (text_length < c->absolute_width - 2) {
    text_input->text[text_length] = (char)key;
    text_input->text[text_length + 1] = '\0';
  }
}


void ctemrui_text_input_delete(void* arg)
{
  ctermui_component c = (ctermui_component)arg;
  if (c->type != TEXT_INPUT) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_text_input: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  char* text = ((TextInput*)c->core_component)->text;
  size_t text_length = strlen(text);
  if (text_length > 0) {
    text[text_length - 1] = '\0';
  }
}

void ctermui_text_input_calculate_absolute_position(ctermui_component c,
                                                    size_t parent_x,
                                                    size_t parent_y,
                                                    size_t parent_width,
                                                    size_t parent_height)
{
    TextInput* text_input = (TextInput*) c->core_component;
    c->absolute_width = (size_t) (((float)text_input->min_width/100.0) * (int)parent_width);
    c->absolute_height = (size_t) (((float)text_input->min_height/100.0) * (int) parent_height);
    c->absolute_x = parent_x;
    c->absolute_y = parent_y;
}

ctermui_component ctermui_new_text_input(
  char* id,
  size_t text_color,
  size_t bg_color,
  size_t min_width,
  size_t min_height,
  ctermui_screen_keyboard_events_t events)
{
  ctermui_component c =
    malloc(sizeof(struct ctermui_component));
  if (c == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for text input component\n");
    exit(EXIT_FAILURE);
  }
  TextInput* text_input_component =
    malloc(sizeof(TextInput));

  if (text_input_component == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for text input component\n");
        exit(EXIT_FAILURE);
  }


  text_input_component->text_color = text_color;
  text_input_component->bg_color = bg_color;
  text_input_component->selected = 0;
  text_input_component->min_width = min_width;
  text_input_component->min_height = min_height;
  c->core_component = text_input_component;
  c->type = TEXT_INPUT;
  strcpy(c->id, id);
  c->draw = ctermui_text_input_draw;
  c->calculate_absolute_position =
    ctermui_text_input_calculate_absolute_position;

  // Add to screen all keyboard events
  ctermui_screen_keyboard_events_register(
    events, 127, ctemrui_text_input_delete, c);

  for (size_t i = 32; i < 127; i++) {
    struct comp_key* ck = malloc(sizeof(struct comp_key));
    if (ck == NULL) {
      fprintf(
        stderr,
        "Error: could not allocate memory for text input component\n");
      exit(EXIT_FAILURE);
    }
    ck->c = c;
    ck->key = i;
    ctermui_screen_keyboard_events_register(
      events, (char)i, ctermui_text_input_write, ck);
    //Todo: find a way to free ck
  }
  return c;
}

void ctermui_soft_background_calculate_absolute_position(
  ctermui_component c,
  size_t parent_x,
  size_t parent_y,
  size_t parent_width,
  size_t parent_height)
{
  c->absolute_x = parent_x;
  c->absolute_y = parent_y;
  c->absolute_width = parent_width;
  c->absolute_height = parent_height;
}

ctermui_component ctermui_new_soft_background(char* id,size_t color)
{
  ctermui_component c =
    malloc(sizeof(struct ctermui_component));
  if (c == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for background component\n");
    exit(EXIT_FAILURE);
  }
  SoftBackground* background_component =
    malloc(sizeof(SoftBackground));

  if (background_component == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for background component\n");
    exit(EXIT_FAILURE);
  }

  background_component->color = color;
  c->core_component = background_component;
  c->type = SOFT_BACKGROUND;
  strcpy(c->id, id);
  c->draw = ctermui_component_draw_soft_background;
  c->calculate_absolute_position =
    ctermui_soft_background_calculate_absolute_position;


  return c;
}

ctermui_component ctermui_new_custom_component(
  char* id,
  void (*draw)(ctermui_screen_t s, ctermui_component c),
  void (*calculate_absolute_position)(ctermui_component c,
                                      size_t parent_x,
                                      size_t parent_y,
                                      size_t parent_width,
                                      size_t parent_height))
{
  ctermui_component c = (ctermui_component)malloc(
    sizeof(struct ctermui_component));
  if (!c) {
    fprintf(
      stderr,
      "Error: could not allocate memory for component\n");
    exit(EXIT_FAILURE);
  }
  c->type = CUSTOM;
  strcpy(c->id, id);
  c->draw = draw;
  c->calculate_absolute_position =
    calculate_absolute_position;
  return c;
}
