#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"

void ctermui_scatter_plot_draw(ctermui_screen_t s, ctermui_component_t c) {
  ScatterPlot *plt = c->core_component;

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

  size_t x_plot_starting = c->x;
  size_t y_plot_starting = c->y;
  size_t x_plot_width = c->width;
  size_t y_plot_height = c->height;
  char tmp[100];
  sprintf(tmp, "%.3f", max_x);
  x_plot_starting += strlen(tmp);
  x_plot_width -= strlen(tmp);
  y_plot_height -= 1;

  ctermui_pencil_draw_line(s->buffer, CTERMUI_VERTICAL, x_plot_starting,
                           y_plot_starting, y_plot_height,
                           CTERMUI_VERTICAL_LINE, plt->fg_color, plt->bg_color);
  ctermui_pencil_draw_line(s->buffer, CTERMUI_HORIZONTAL, x_plot_starting,
                           y_plot_starting + y_plot_height - 1, x_plot_width,
                           CTERMUI_HORIZONTAL_LINE, plt->fg_color,
                           plt->bg_color);

  for (size_t i = 0; i < plt->size; ++i) {
    float scaled_x, scaled_y;
    int x_position, y_position;

    if (min_x == max_x) {
      scaled_x = 0.3f * (x_plot_width - 1);
      x_position = x_plot_starting + (int)scaled_x;
    } else {
      scaled_x =
          (plt->xvalues[i] - min_x) / (max_x - min_x) * (x_plot_width - 1);
      x_position = x_plot_starting + (int)scaled_x;
    }

    if (min_y == max_y) {
      scaled_y = 0.3f * (y_plot_height - 1);
      y_position = y_plot_starting + (int)scaled_y;
    } else {
      scaled_y =
          (plt->yvalues[i] - min_y) / (max_y - min_y) * (y_plot_height - 1);
      y_position = y_plot_starting + y_plot_height - 1 - (int)scaled_y;
    }

    ctermui_pencil_draw_char(
        s->buffer, x_position, y_plot_starting + y_plot_height - 1,
        CTERMUI_VERTICAL_LINE, plt->fg_color, plt->bg_color, 0);
    ctermui_pencil_draw_char(s->buffer, x_plot_starting, y_position,
                             CTERMUI_HORIZONTAL_LINE, plt->fg_color,
                             plt->bg_color, 0);

    char x_label[100];
    sprintf(x_label, "%.3f", plt->xvalues[i]);
    char y_label[100];
    sprintf(y_label, "%.3f", plt->yvalues[i]);
    ctermui_pencil_draw_text(s->buffer, c->x, y_position, y_label,
                             plt->fg_color, plt->bg_color);

    size_t label_pos = x_position - strlen(x_label) / 2;
    if (label_pos + strlen(x_label) > c->x + c->width) {
      label_pos = c->x + c->width - strlen(x_label);
    }
    ctermui_pencil_draw_text(s->buffer, label_pos, c->y + c->height - 1,
                             x_label, plt->fg_color, plt->bg_color);
  }

  for (size_t i = 0; i < plt->size; ++i) {
    float scaled_x, scaled_y;
    int x_position, y_position;

    if (min_x == max_x) {
      scaled_x = 0.3f * (x_plot_width - 1);
      x_position = x_plot_starting + (int)scaled_x;
    } else {
      scaled_x =
          (plt->xvalues[i] - min_x) / (max_x - min_x) * (x_plot_width - 1);
      x_position = x_plot_starting + (int)scaled_x;
    }

    if (min_y == max_y) {
      scaled_y = 0.3f * (y_plot_height - 1);
      y_position = y_plot_starting + (int)scaled_y;
    } else {
      scaled_y =
          (plt->yvalues[i] - min_y) / (max_y - min_y) * (y_plot_height - 1);
      y_position = y_plot_starting + y_plot_height - 1 - (int)scaled_y;
    }
    ctermui_pencil_draw_char(s->buffer, x_position, y_position,
                             plt->point_symbol, plt->point_color, plt->bg_color,
                             0);
    if (plt->line_linking) {
      if (i > 0) {
        float scaled_x_prev, scaled_y_prev;
        int x_position_prev, y_position_prev;

        if (min_x == max_x) {
          scaled_x_prev = 0.3f * (x_plot_width - 1);
          x_position_prev = x_plot_starting + (int)scaled_x_prev;
        } else {
          scaled_x_prev = (plt->xvalues[i - 1] - min_x) / (max_x - min_x) *
                          (x_plot_width - 1);
          x_position_prev = x_plot_starting + (int)scaled_x_prev;
        }

        if (min_y == max_y) {
          scaled_y_prev = 0.3f * (y_plot_height - 1);
          y_position_prev = y_plot_starting + (int)scaled_y_prev;
        } else {
          scaled_y_prev = (plt->yvalues[i - 1] - min_y) / (max_y - min_y) *
                          (y_plot_height - 1);
          y_position_prev =
              y_plot_starting + y_plot_height - 1 - (int)scaled_y_prev;
        }
        ctermui_pencil_draw_line_with_points(
            s->buffer, x_position_prev, y_position_prev, x_position, y_position,
            plt->point_color, plt->bg_color, plt->point_symbol);
      }
    }
  }
}

void ctermui_scatter_plot_calculate_absolute_position(ctermui_component_t c,
                                                      size_t parent_x,
                                                      size_t parent_y,
                                                      size_t parent_width,
                                                      size_t parent_height) {
  c->x = parent_x;
  c->y = parent_y;
  c->width = parent_width;
  c->height = parent_height;
}

ctermui_component_t ctermui_new_scatter_plot(char *id, float *xvalues,
                                             float *yvalues, size_t values_size,
                                             uint8_t point_color,
                                             uint8_t bg_color, uint8_t fg_color,
                                             char point_symbol,
                                             uint8_t line_linking) {
  ScatterPlot *scatter_plot_component = malloc(sizeof(ScatterPlot));
  scatter_plot_component->bg_color = bg_color;
  scatter_plot_component->fg_color = fg_color;
  scatter_plot_component->point_color = point_color;
  scatter_plot_component->xvalues = xvalues;
  scatter_plot_component->yvalues = yvalues;
  scatter_plot_component->size = values_size;
  scatter_plot_component->point_symbol = point_symbol;
  scatter_plot_component->line_linking = line_linking;
  return ctermui_new_custom_component(
      id, scatter_plot_component, ctermui_scatter_plot_draw,
      ctermui_scatter_plot_calculate_absolute_position);
}
