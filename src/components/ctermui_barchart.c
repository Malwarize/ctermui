#include "ctermui_component.h"
#include "ctermui_screen.h"

/**
 * @brief Calculate absolute positions for all bars and labels in the bar chart.
 *
 * This function updates the position and size of each bar and label in the bar chart
 * based on the parent layout's dimensions and the chart's orientation.
 *
 * @param c Pointer to the bar chart component.
 * @param parent_x X coordinate of the parent layout.
 * @param parent_y Y coordinate of the parent layout.
 * @param parent_width Width of the parent layout.
 * @param parent_height Height of the parent layout.
 */
void ctermui_barchart_calculate_absolute_position(ctermui_component_t c,
                                                  size_t parent_x,
                                                  size_t parent_y,
                                                  size_t parent_width,
                                                  size_t parent_height) {
  BarChart *barchart = c->core_component;
  if (barchart->orientation == CTERMUI_HORIZONTAL) {
    float bar_height_float = (float)parent_height / barchart->values_count;
    for (size_t i = 0; i < barchart->values_count; ++i) {
      ctermui_component_t bar = barchart->bars[i];
      ctermui_component_t label = barchart->labels[i];

      int bar_height = (int)(bar_height_float - barchart->gap);
      if (bar_height <= 0) {
        bar_height = 1;
      }

      int y_position = parent_y + (int)(i * bar_height_float);
      if (y_position < 0) {
        y_position = parent_y;
      }

      label->calculate_absolute_position(label, parent_x, y_position,
                                         barchart->_max_text_width, bar_height);

      bar->calculate_absolute_position(
          bar, parent_x + barchart->_max_text_width, y_position,
          parent_width - barchart->_max_text_width - 1, bar_height);
    }
  } else {
    float bar_width_float = (float)parent_width / (float)barchart->values_count;
    for (size_t i = 0; i < barchart->values_count; ++i) {
      ctermui_component_t bar = barchart->bars[i];
      ctermui_component_t label = barchart->labels[i];
      label->height = 3;
      int bar_width = (int)(bar_width_float - barchart->gap);
      if (bar_width <= 0) {
        bar_width = 1;
      }

      int x_position = parent_x + (int)(i * bar_width_float);
      if (x_position < 0) {
        x_position = parent_x;
      }

      label->calculate_absolute_position(
          label, x_position, parent_y + parent_height - label->height,
          bar_width, label->height);

      bar->calculate_absolute_position(bar, x_position, parent_y, bar_width,
                                       parent_height - label->height);
    }
  }
}

/**
 * @brief Draw the bar chart component (all bars and labels).
 *
 * @param s Pointer to the screen structure.
 * @param c Pointer to the bar chart component.
 */
void ctermui_barchart_draw(ctermui_screen_t s, ctermui_component_t c) {
  BarChart *barchart = c->core_component;
  for (size_t i = 0; i < barchart->values_count; ++i) {
    ctermui_component_t bar = barchart->bars[i];
    ctermui_component_t label = barchart->labels[i];
    bar->draw(s, bar);
    label->draw(s, label);
  }
}

/**
 * @brief Find the length of the longest label in the bar chart.
 *
 * @param labels Array of label strings.
 * @param values_length Number of labels.
 * @return Length of the longest label.
 */
size_t biggest_text_length(char (*labels)[100], size_t values_length) {
  size_t max = 0;
  for (size_t i = 0; i < values_length; ++i) {
    size_t len = strlen(labels[i]);
    if (len > max) {
      max = len;
    }
  }
  return max;
}

/**
 * @brief Create a new bar chart component.
 *
 * Allocates and initializes a new bar chart with the given parameters, including bars and labels.
 *
 * @param id String identifier for the bar chart.
 * @param bar_color Color of the bars.
 * @param bg_color Background color.
 * @param max Maximum value for the bars.
 * @param orientation Orientation (CTERMUI_HORIZONTAL or CTERMUI_VERTICAL).
 * @param values Array of values for each bar.
 * @param labels Array of label strings for each bar.
 * @param values_length Number of bars/labels.
 * @param gap Gap between bars.
 * @return Pointer to the new bar chart component. Exits on allocation failure.
 * @note Caller is responsible for freeing the component (if a free function exists).
 */
ctermui_component_t ctemrui_new_barchart(char *id, int8_t bar_color,
                                         int8_t bg_color, size_t max,
                                         size_t orientation, int *values,
                                         char (*lables)[100],
                                         size_t values_length, int gap) {
  BarChart *barchart_component = malloc(sizeof(BarChart));
  if (barchart_component == NULL) {
    fprintf(stderr,
            "Error: could not allocate memory for barchart component\n");
    exit(EXIT_FAILURE);
  }
  barchart_component->values_count = values_length;
  barchart_component->orientation = orientation;
  barchart_component->gap = gap;
  barchart_component->_max_text_width =
      biggest_text_length(lables, values_length);
  char id_buff[100];
  for (size_t i = 0; i < values_length; ++i) {
    sprintf(id_buff, "%s_bar_%zu", id, i);
    ctermui_component_t bar =
        ctermui_new_progress_bar(id_buff, bar_color, bg_color, max, values[i],
                                 "", CTERMUI_EMPTY, orientation);
    barchart_component->bars[i] = bar;

    // lables
    sprintf(id_buff, "%s_label_%zu", id, i);
    int align;
    if (orientation == CTERMUI_HORIZONTAL) {
      align = CTERMUI_ALIGN_CENTER;
    } else {
      align = CTERMUI_ALIGN_BOTTOM;
    }
    ctermui_component_t label = ctermui_new_text(
        id_buff, lables[i], CTERMUI_EMPTY, CTERMUI_EMPTY, align);
    barchart_component->labels[i] = label;
  }

  return ctermui_new_custom_component(
      id, barchart_component, ctermui_barchart_draw,
      ctermui_barchart_calculate_absolute_position);
}
