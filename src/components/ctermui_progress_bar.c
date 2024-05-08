#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"
void ctermui_barchart_update_values(ctermui_component_t c, int *values,
                                    size_t values_length) {

    BarChart *barchart = c->core_component;
    for (size_t i = 0; i < values_length; ++i) {
        ProgressBar *pb = barchart->bars[i]->core_component;
        pb->progress = values[i];
    }
}


void ctermui_progress_bar_update_value(ctermui_component_t c, size_t value) {
    ProgressBar *progress_bar = c->core_component;
    progress_bar->progress = value;
}

void ctermui_progress_bar_calculate_absolute_position(ctermui_component_t c,
                                                      size_t parent_x,
                                                      size_t parent_y,
                                                      size_t parent_width,
                                                      size_t parent_height) {
    c->x = parent_x;
    c->y = parent_y;
    c->width = parent_width;
    c->height = parent_height;
}

void ctermui_component_draw_progress_bar(ctermui_screen_t s,
                                         ctermui_component_t c) {
  ProgressBar *progress_bar = c->core_component;
  if (progress_bar->orientation == CTERMUI_HORIZONTAL) {
    ctermui_pencil_solid_background(s->buffer, c->x, c->y,
                                    c->width, c->height,
                                    progress_bar->bg_color);
    size_t progress_width =
        (size_t)((float)progress_bar->progress / (float)progress_bar->max *
                 (float)c->width);
    ctermui_pencil_solid_background(s->buffer, c->x, c->y,
                                    progress_width, c->height,
                                    progress_bar->bar_color);
    size_t text_width = strlen(progress_bar->text);
    size_t text_x = c->x + (c->width - text_width) / 2;
    size_t text_y = c->y + (c->height - 1) / 2;
    for (size_t i = 0; i < text_width; ++i) {
      if (i + text_x < progress_width) {
        ctermui_pencil_draw_char(
            s->buffer, text_x + i, text_y, progress_bar->text[i],
            progress_bar->text_color, progress_bar->bar_color, 0);
      } else {
        ctermui_pencil_draw_char(
            s->buffer, text_x + i, text_y, progress_bar->text[i],
            progress_bar->text_color, progress_bar->bg_color, 0);
      }
    }
  } else {
    ctermui_pencil_solid_background(s->buffer, c->x, c->y,
                                    c->width, c->height,
                                    progress_bar->bg_color);
    size_t progress_height =
        (size_t)((float)progress_bar->progress / (float)progress_bar->max *
                 (float)c->height);
    ctermui_pencil_solid_background(
        s->buffer, c->x,
        c->y + c->height - progress_height, c->width,
        progress_height, progress_bar->bar_color);
    size_t text_width = strlen(progress_bar->text);
    size_t text_x = c->x + (c->width - text_width) / 2;
    size_t text_y = c->y + (c->height - 1) / 2;
    for (size_t i = 0; i < text_width; ++i) {
      if (i + text_x < progress_height) {
        ctermui_pencil_draw_char(
            s->buffer, text_x + i, text_y, progress_bar->text[i],
            progress_bar->text_color, progress_bar->bar_color, 0);
      } else {
        ctermui_pencil_draw_char(
            s->buffer, text_x + i, text_y, progress_bar->text[i],
            progress_bar->text_color, progress_bar->bg_color, 0);
      }
    }
  }
}


ctermui_component_t ctermui_new_progress_bar(char *id, int8_t bar_color,
int8_t bg_color, size_t max,
size_t progress, char *text,
int8_t text_color,
size_t orientation) {
    ProgressBar *progress_bar_component = malloc(sizeof(ProgressBar));

    if (progress_bar_component == NULL) {
        fprintf(stderr,
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
    return ctermui_new_custom_component(id, progress_bar_component, ctermui_component_draw_progress_bar, ctermui_progress_bar_calculate_absolute_position);
}

