#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"
void ctermui_component_draw_button(ctermui_screen_t s,
                                   ctermui_component_t c)
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
                                  ctermui_component_t c)
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
                                  ctermui_component_t c)
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
  ctermui_screen_t s, ctermui_component_t c)
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
      ctermui_pencil_draw_char(
        s->buffer,
        x,
        y,
        s->buffer[x][y]->character,
        s->buffer[x][y]->foreground_color,
        background->color,
        0);
    }
  }
}

void ctermui_component_draw_soft_background(
  ctermui_screen_t s, ctermui_component_t c)
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
  ctermui_component_t c,
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

ctermui_component_t ctermui_new_button(char* id,
                                     char* text,
                                     size_t align,
                                     int8_t text_color,
                                     int8_t bg_color)
{
  ctermui_component_t c =
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
  strcpy(c->id, id);
  c->draw = ctermui_component_draw_button;
  c->calculate_absolute_position =
    ctermui_button_calculate_absolute_position;
  return c;
}

void ctermui_text_calculate_absolute_position(
  ctermui_component_t c,
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

ctermui_component_t ctermui_new_text(
  char* id, char* text, int8_t color, int8_t bg_color, size_t align)
{
  ctermui_component_t c =
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
  strcpy(c->id, id);
  c->draw = ctermui_component_draw_label;
  c->calculate_absolute_position = ctermui_text_calculate_absolute_position;
  return c;
}

void ctermui_calculate_absolute_position(
  ctermui_component_t c,
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

ctermui_component_t ctermui_new_frame(char* id,
                                    int8_t color,
                                    int8_t bg_color)
{
  ctermui_component_t c =
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
  ctermui_component_t c,
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

ctermui_component_t ctermui_new_solid_background(char* id,
                                               int8_t color,
                                               size_t width,
                                               size_t height)
{
  ctermui_component_t c =
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
  strcpy(c->id, id);
  c->draw = ctermui_component_draw_solid_background;
  c->calculate_absolute_position =
    ctermui_solid_background_calculate_absolute_position;
  return c;
}
void ctermui_progress_bar_update_value(ctermui_component_t c,
                                     size_t value)
{
  if (c->type != PROGRESS_BAR) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_progress_bar: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  ProgressBar* progress_bar = (ProgressBar*)c->core_component;
  progress_bar->progress = value;
}

void ctermui_progress_bar_calculate_absolute_position(
  ctermui_component_t c,
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
  ctermui_screen_t s, ctermui_component_t c)
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
                                 progress_bar->bar_color,
                                 0);
      }
      else {
        ctermui_pencil_draw_char(s->buffer,
                                 text_x + i,
                                 text_y,
                                 progress_bar->text[i],
                                 progress_bar->text_color,
                                 progress_bar->bg_color,
                                 0);
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
                                 progress_bar->bar_color,
                                 0);
      }
      else {
        ctermui_pencil_draw_char(s->buffer,
                                 text_x + i,
                                 text_y,
                                 progress_bar->text[i],
                                 progress_bar->text_color,
                                 progress_bar->bg_color,
                                 0);
      }
    }
  }
}

ctermui_component_t ctermui_new_progress_bar(char* id,
                                           int8_t bar_color,
                                           int8_t bg_color,
                                           size_t max,
                                           size_t progress,
                                           char* text,
                                           int8_t text_color,
                                           size_t orientation)
{
  ctermui_component_t c =
    malloc(sizeof(struct ctermui_component));
  if (c == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for progress bar component\n");
    exit(EXIT_FAILURE);
  }
  ProgressBar* progress_bar_component = malloc(sizeof(ProgressBar));

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
  strcpy(c->id, id);
  c->draw = ctermui_component_draw_progress_bar;
  c->calculate_absolute_position =
    ctermui_progress_bar_calculate_absolute_position;
  return c;
}

void ctermui_barchart_update_values(ctermui_component_t c,
                                    int* values,
                                    size_t values_length)
{
  if (c->type != BARCHART) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_barchart: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  BarChart* barchart = (BarChart*)c->core_component;
  for (size_t i = 0; i < values_length; ++i) {
    ProgressBar* pb = (ProgressBar*)barchart->bars[i]->core_component;
    pb->progress = values[i];
  }
}

void ctermui_barchart_draw(ctermui_screen_t s,
                           ctermui_component_t c)
{
  if (c->type != BARCHART) {
    fprintf(
      stderr,
      "ctermui_pencil_draw_barchart: invalid component type\n");
    exit(EXIT_FAILURE);
  }
  
  BarChart* barchart = (BarChart*)c->core_component;
  for (size_t i = 0; i < barchart->values_count; ++i) {
    ctermui_component_t bar = barchart->bars[i];
    ctermui_component_t label = barchart->labels[i];
    bar->draw(s, bar);
    label->draw(s, label);
  }
}
size_t biggest_text_length(char (*labels)[100], size_t values_length)
{
  size_t max = 0;
  for (size_t i = 0; i < values_length; ++i) {
    size_t len = strlen(labels[i]);
    if (len > max) {
      max = len;
    }
  }
  return max;
}

void ctermui_barchart_calculate_absolute_position(
  ctermui_component_t c,
  size_t parent_x,
  size_t parent_y,
  size_t parent_width,
  size_t parent_height)
{
  BarChart* barchart = (BarChart*)c->core_component;
  if(barchart->orientation == CTERMUI_HORIZONTAL){
    int bar_height = (int)((float)parent_height / barchart->values_count);
    for (size_t i = 0; i < barchart->values_count; ++i) {
      ctermui_component_t bar = barchart->bars[i];
      ctermui_component_t label = barchart->labels[i];
      // for horizontal barchart we need to calculate the max text width 
      int gap;
      if(bar_height - barchart->gap <= 0){
        gap = 0;
      }else{
        gap = 1;
      }
      label->calculate_absolute_position(
        label,
        parent_x,
        parent_y + i*bar_height,
        barchart->_max_text_width,
        bar_height - gap
      );

      bar->calculate_absolute_position(
        bar,
        parent_x + barchart->_max_text_width,
        parent_y + i*bar_height,
        parent_width - barchart->_max_text_width,
        bar_height - gap
      );
    }
  }else{
    int bar_width = (int) ((float) parent_width / (float) barchart->values_count);
    for (size_t i = 0; i < barchart->values_count; ++i) {
      ctermui_component_t bar = barchart->bars[i];
      ctermui_component_t label = barchart->labels[i];
      bar->calculate_absolute_position(
        bar,
        parent_x + i*bar_width,
        parent_y,
        bar_width-barchart->gap,
        parent_y + parent_height-label->absolute_height
      );
      label->calculate_absolute_position(
        label,
        parent_x + i*bar_width,
        parent_y + parent_height - label->absolute_height,
        bar_width-barchart->gap,
        label->absolute_height
      );
    }
  
  }
}

    
ctermui_component_t  ctemrui_new_barchart(char* id,
                                        int8_t bar_color,
                                        int8_t bg_color,
                                        size_t max,
                                        size_t orientation,
                                        int* values,
                                        char (*lables)[100],
                                        size_t values_length,
                                        int gap
                                        )
{
  ctermui_component_t c =
    malloc(sizeof(struct ctermui_component));
  if (c == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for barchart component\n");
    exit(EXIT_FAILURE);
  }
  BarChart* barchart_component = malloc(sizeof(BarChart));
  if (barchart_component == NULL) {
    fprintf(
      stderr,
      "Error: could not allocate memory for barchart component\n");
    exit(EXIT_FAILURE);
  }
  barchart_component->values_count = values_length;
  barchart_component->orientation = orientation;
  barchart_component->gap = gap;
  barchart_component->_max_text_width = biggest_text_length(lables, values_length);
  char id_buff[100];
  for (size_t i = 0; i < values_length; ++i) {
    sprintf(id_buff, "%s_bar_%zu", id, i);
    ctermui_component_t bar = ctermui_new_progress_bar(
      id_buff, bar_color, bg_color, max, values[i], "", CTERMUI_EMPTY, orientation);
    barchart_component->bars[i] = bar;
  
    // lables 
    sprintf(id_buff, "%s_label_%zu", id, i);
    ctermui_component_t label = ctermui_new_text(
      id_buff, lables[i], CTERMUI_EMPTY, CTERMUI_EMPTY, CTERMUI_ALIGN_CENTER);
    barchart_component->labels[i] = label;
  
  }
  
  
  c->core_component = barchart_component;
  c->type = BARCHART;
  c->calculate_absolute_position = ctermui_barchart_calculate_absolute_position;
  c->draw = ctermui_barchart_draw;
  strcpy(c->id, id);
  return c;
}

void ctermui_scatter_plot_draw(ctermui_screen_t s, ctermui_component_t c) {
    ScatterPlot* plt = (ScatterPlot*)c->core_component;

    float max_x = plt->xvalues[0];
    float max_y = plt->yvalues[0];
    float min_x = plt->xvalues[0];
    float min_y = plt->yvalues[0];

  for (size_t i = 1; i < plt->size; ++i) {
        if (plt->xvalues[i] > max_x) {
            max_x = plt->xvalues[i];
        }
        if (plt->yvalues[i] > max_y) {
            max_y = plt->yvalues[i];
        }
        if (plt->xvalues[i] < min_x && plt->xvalues[i] != 0.0f) {
            min_x = plt->xvalues[i];
        }
        if (plt->yvalues[i] < min_y && plt->yvalues[i] != 0.0f) {
            min_y = plt->yvalues[i];
        }
    }

    if (min_x == max_x) {
        min_x = 0.0f;
        max_x = 1.0f;
    }

    if (min_y == max_y) {
        min_y = 0.0f;
        max_y = 1.0f;
    }

    size_t x_plot_starting = c->absolute_x;
    size_t y_plot_starting = c->absolute_y;
    size_t x_plot_width = c->absolute_width;
    size_t y_plot_height = c->absolute_height;
    char tmp[5];
    sprintf(tmp, "%.2f", max_x);
    x_plot_starting += strlen(tmp);
    x_plot_width -= strlen(tmp);
    y_plot_height -= 1;

    ctermui_pencil_draw_line(s->buffer, CTERMUI_VERTICAL, x_plot_starting, y_plot_starting, y_plot_height, CTERMUI_VERTICAL_LINE, plt->fg_color, plt->bg_color);
    ctermui_pencil_draw_line(s->buffer, CTERMUI_HORIZONTAL, x_plot_starting, y_plot_starting + y_plot_height - 1, x_plot_width, CTERMUI_HORIZONTAL_LINE, plt->fg_color, plt->bg_color);

    for (size_t i = 0; i < plt->size; ++i) {
        if (plt->xvalues[i] == 0.0f || plt->yvalues[i] == 0.0f) {
            continue; // Skip (0,0) if it doesn't exist
        }

        float scaled_x = (plt->xvalues[i] - min_x) / (max_x - min_x) * (x_plot_width - 1);
        float scaled_y = (plt->yvalues[i] - min_y) / (max_y - min_y) * (y_plot_height - 1);
        int x_position = x_plot_starting + (int)scaled_x;
        int y_position = y_plot_starting + y_plot_height - 1 - (int)scaled_y;

        ctermui_pencil_draw_char(s->buffer, x_position, y_plot_starting + y_plot_height - 1, CTERMUI_VERTICAL_LINE, plt->fg_color, plt->bg_color, 0);
        ctermui_pencil_draw_char(s->buffer, x_plot_starting, y_position, CTERMUI_HORIZONTAL_LINE, plt->fg_color, plt->bg_color, 0);

        char x_label[100];
        sprintf(x_label, "%.2f", plt->xvalues[i]);
        char y_label[100];
        sprintf(y_label, "%.2f", plt->yvalues[i]);
        ctermui_pencil_draw_text(s->buffer, c->absolute_x, y_position, y_label, plt->fg_color, plt->bg_color);

        size_t label_pos = x_position - strlen(x_label) / 2;
        if (label_pos + strlen(x_label) > c->absolute_x + c->absolute_width) {
            label_pos = c->absolute_x + c->absolute_width - strlen(x_label);
        }
        ctermui_pencil_draw_text(s->buffer, label_pos, c->absolute_y + c->absolute_height - 1, x_label, plt->fg_color, plt->bg_color);
    }

  for (size_t i = 0; i < plt->size; ++i) {
      float scaled_x, scaled_y;
      int x_position, y_position;

      if (min_x == max_x) {
          scaled_x = 0.5f * (x_plot_width - 1);
          x_position = x_plot_starting + (int)scaled_x;
      } else {
          scaled_x = (plt->xvalues[i] - min_x) / (max_x - min_x) * (x_plot_width - 1);
          x_position = x_plot_starting + (int)scaled_x;
      }

      if (min_y == max_y) {
          scaled_y = 0.5f * (y_plot_height - 1);
          y_position = y_plot_starting + (int)scaled_y;
      } else {
          scaled_y = (plt->yvalues[i] - min_y) / (max_y - min_y) * (y_plot_height - 1);
          y_position = y_plot_starting + y_plot_height - 1 - (int)scaled_y;
      }

      ctermui_pencil_draw_char(s->buffer, x_position, y_position, plt->point_symbol, plt->point_color, plt->bg_color, 0);
  }

    if (plt->line_linking) {
        for (size_t i = 0; i < plt->size - 1; ++i) {
            if (plt->xvalues[i] == 0.0f || plt->yvalues[i] == 0.0f || plt->xvalues[i + 1] == 0.0f || plt->yvalues[i + 1] == 0.0f) {
                continue; // Skip lines involving (0,0) if they don't exist
            }

            float scaled_x1 = (plt->xvalues[i] - min_x) / (max_x - min_x) * (x_plot_width - 1);
            float scaled_y1 = (plt->yvalues[i] - min_y) / (max_y - min_y) * (y_plot_height - 1);
            int x1 = x_plot_starting + (int)scaled_x1;
            int y1 = y_plot_starting + y_plot_height - 1 - (int)scaled_y1;

            float scaled_x2 = (plt->xvalues[i + 1] - min_x) / (max_x - min_x) * (x_plot_width - 1);
            float scaled_y2 = (plt->yvalues[i + 1] - min_y) / (max_y - min_y) * (y_plot_height - 1);
            int x2 = x_plot_starting + (int)scaled_x2;
            int y2 = y_plot_starting + y_plot_height - 1 - (int)scaled_y2;

            ctermui_pencil_draw_line_with_points(s->buffer, x1, y1, x2, y2, plt->point_color, plt->bg_color, plt->point_symbol);
        }
    }
}



void ctermui_scatter_plot_calculate_absolute_position(  
  ctermui_component_t c,
  size_t parent_x,
  size_t parent_y,
  size_t parent_width,
  size_t parent_height
)
{
  c->absolute_x = parent_x;
  c->absolute_y = parent_y;
  c->absolute_width = parent_width;
  c->absolute_height = parent_height;
}

ctermui_component_t ctermui_new_scatter_plot(
  char* id,
  float* xvalues,
  float* yvalues,
  size_t values_size,
  uint8_t point_color,
  uint8_t bg_color,
  uint8_t fg_color,
  char point_symbol,
  uint8_t line_linking 
)
{
  ctermui_component_t c =  malloc(sizeof(struct ctermui_component));
  if(!c){
    fprintf(
      stderr,
      "Error: could not allocate memory for scatter_plot component\n");
    exit(EXIT_FAILURE);
  }
  ScatterPlot* scatter_plot_component = malloc(sizeof(ScatterPlot));
  scatter_plot_component->bg_color = bg_color;
  scatter_plot_component->fg_color = fg_color;
  scatter_plot_component->point_color = point_color;
  scatter_plot_component->xvalues = xvalues;
  scatter_plot_component->yvalues = yvalues;
  scatter_plot_component->size = values_size;
  scatter_plot_component->point_symbol = point_symbol;
  scatter_plot_component->line_linking = line_linking;
  c->core_component = scatter_plot_component;
  c->type = SCATTER_PLOT;
  c->draw = ctermui_scatter_plot_draw;
  c->calculate_absolute_position = ctermui_scatter_plot_calculate_absolute_position;
}

void ctermui_text_input_draw(ctermui_screen_t s,
                             ctermui_component_t c)
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
  ctermui_component_t c;
  size_t key;
};

void ctermui_text_input_write(void* args)
{
  struct comp_key* ck = (struct comp_key*)args;
  ctermui_component_t c = ck->c;
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
  ctermui_component_t c = (ctermui_component_t)arg;
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

void ctermui_text_input_calculate_absolute_position(
  ctermui_component_t c,
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

ctermui_component_t ctermui_new_text_input(
  char* id,
  int8_t text_color,
  int8_t bg_color,
  size_t min_width,
  size_t min_height,
  ctermui_screen_keyboard_events_t events)
{
  ctermui_component_t c =
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
  ctermui_component_t c,
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



ctermui_component_t ctermui_new_soft_background(char* id,int8_t color)
{
  ctermui_component_t c =
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

ctermui_component_t ctermui_new_custom_component(
  char* id,
  void (*draw)(ctermui_screen_t s,
               ctermui_component_t c), // draw
  void (*calculate_absolute_position)(
    ctermui_component_t c, // calculate_absolute_position
                                      size_t parent_x,
                                      size_t parent_y,
                                      size_t parent_width,
                                      size_t parent_height),
  void* core_component
  )
{
  ctermui_component_t c = (ctermui_component_t)malloc(
    sizeof(struct ctermui_component));
  if (!c) {
    fprintf(
      stderr,
      "Error: could not allocate memory for component\n");
    exit(EXIT_FAILURE);
  }

  c->core_component = core_component;
  c->type = CUSTOM;
  strcpy(c->id, id);
  c->draw = draw;
  c->calculate_absolute_position =
    calculate_absolute_position;
  c->core_component = core_component;
  return c;
}

