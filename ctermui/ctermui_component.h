#ifndef CTERMUI_COMPONENT_H
#define CTERMUI_COMPONENT_H
typedef struct ctermui_screen* ctermui_screen_t;
typedef struct ctermui_widget* ctermui_widget_t;
#include "ctermui_events.h"
#include <signal.h>
#include <stdint.h>
#include <unistd.h>


typedef struct ctermui_component {
  char id[100];
  uint16_t type;
  size_t absolute_x;
  size_t absolute_y;
  size_t absolute_width;
  size_t absolute_height;
  void (*draw)(ctermui_screen_t s,
               struct ctermui_component* c);
  void (*calculate_absolute_position)(
    struct ctermui_component* c,
    size_t parent_x,
    size_t parent_y,
    size_t parent_width,
    size_t parent_height);
  void* core_component;
  ctermui_widget_t parent;
}* ctermui_component_t;

enum CTYPES {
  TEXT,
  BUTTON,
  FRAME,
  SOFT_BACKGROUND,
  SOLID_BACKGROUND,
  PROGRESS_BAR,
  TEXT_INPUT,
  BARCHART,
  SCATTER_PLOT,
  CUSTOM
};

enum ALIGN {
  CTERMUI_ALIGN_LEFT_CENTER,
  CTERMUI_ALIGN_RIGHT_CENTER,
  CTERMUI_ALIGN_CENTER,
  CTERMUI_ALIGN_LEFT_TOP,
  CTERMUI_ALIGN_RIGHT_TOP,
  CTERMUI_ALIGN_TOP,
  CTERMUI_ALIGN_LEFT_BOTTOM,
  CTERMUI_ALIGN_RIGHT_BOTTOM,
  CTERMUI_ALIGN_BOTTOM,
  CTERMUI_ALIGN_TOP_CENTER,
  CTERMUI_ALIGN_BOTTOM_CENTER
};

typedef struct {
  char text[100];
  int8_t color;
  int8_t bg_color;

  size_t align;
} Text;

typedef struct {
  int8_t color;
  int8_t bg_color;

} Frame;

typedef struct {
  int8_t color;
} SoftBackground;

typedef struct {
  int8_t color;
} SolidBackground;

typedef struct {
  char text[100];
  size_t align;
  int8_t text_color;
  int8_t bg_color;
} Button;

#define PROGRESS_BAR_CHAR "█"

typedef struct {
  char symbol;  // default is █
  int8_t bar_color;
  int8_t bg_color;
  size_t max;
  size_t progress;
  char* text;
  int8_t text_color;
  size_t orientation;  // 0 = horizontal, 1 = vertical
} ProgressBar;

#define MAX_BARS 100
typedef struct {
  size_t values_count;
  ctermui_component_t bars[MAX_BARS];
  ctermui_component_t labels[MAX_BARS];
  uint8_t orientation;
  size_t gap;
  size_t _max_text_width;
} BarChart;

#define MAX_LABELS 100

typedef struct {
  float* xvalues;
  float* yvalues;
  char (*xlabels)[MAX_LABELS];
  char (*ylabels)[MAX_LABELS];
  int8_t bg_color;
  int8_t fg_color;
  int8_t point_color;
  size_t size;
  char point_symbol;
  uint8_t line_linking;
} ScatterPlot;

#define MAX_TEXT_INPUT_LENGTH 100
typedef struct {
  char text[MAX_TEXT_INPUT_LENGTH];
  size_t align;
  int8_t text_color;
  int8_t bg_color;
  int8_t selected;
  size_t min_width;
  size_t min_height;
} TextInput;

ctermui_component_t ctermui_new_button(char* id,
                                     char* text,
                                     size_t align,
                                     int8_t text_color,
                                     int8_t bg_color);

ctermui_component_t ctermui_new_text(
  char* id, char* text, int8_t color, int8_t bg_color, size_t align);

ctermui_component_t ctermui_new_frame(char* id,
                                    int8_t color,
                                    int8_t bg_color);

ctermui_component_t ctermui_new_solid_background(char* id,
                                               int8_t color,
                                               size_t width,
                                               size_t height);

void ctermui_progress_bar_update_value(ctermui_component_t c,
                                     size_t value);

ctermui_component_t ctermui_new_soft_background(char* id,
                                              int8_t color);

ctermui_component_t ctermui_new_progress_bar(char* id,
                                           int8_t bar_color,
                                           int8_t bg_color,
                                           size_t max,
                                           size_t progress,
                                           char* text,
                                           int8_t text_color,
                                           size_t orientation);

ctermui_component_t ctermui_new_custom_component(
  char* id,
  void (*draw)(ctermui_screen_t s, ctermui_component_t c),
  void (*calculate_absolute_position)(ctermui_component_t c,
                                      size_t parent_x,
                                      size_t parent_y,
                                      size_t parent_width,
                                      size_t parent_height),
  void* core_component
  );
void ctermui_component_draw_button(ctermui_screen_t s,
                                   ctermui_component_t c);

void ctermui_component_draw_label(ctermui_screen_t s,
                                  ctermui_component_t c);

void ctermui_component_draw_frame(ctermui_screen_t s,
                                  ctermui_component_t c);

void ctermui_component_draw_solid_background(
  ctermui_screen_t s, ctermui_component_t c);

ctermui_component_t ctermui_new_text_input(
  char* id,
  int8_t text_color,
  int8_t bg_color,
  size_t min_width,
  size_t min_height,
  ctermui_screen_keyboard_events_t events);

void ctermui_component_draw_soft_background(
  ctermui_screen_t s, ctermui_component_t c);

void ctermui_barchart_update_values(ctermui_component_t c,
                                    int* values,
                                    size_t values_length);

ctermui_component_t  ctemrui_new_barchart(char* id,
                                        int8_t bar_color,
                                        int8_t bg_color,
                                        size_t max,
                                        size_t orientation,
                                        int* values,
                                        char(*labels)[100],
                                        size_t values_length,
                                        int gap
                                        );
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
);
#endif  // CTERMUI_COMPONENT_H
