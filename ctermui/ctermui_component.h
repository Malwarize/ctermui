#ifndef CTERMUI_COMPONENT_H
#define CTERMUI_COMPONENT_H
typedef struct ctermui_screen* ctermui_screen_t;
typedef struct ctermui_widget* ctermui_widget_t;
#include "ctermui_events.h"
#include <signal.h>
#include <stdint.h>
#include <unistd.h>

enum CTYPES {
  TEXT,
  BUTTON,
  FRAME,
  SOFT_BACKGROUND,
  SOLID_BACKGROUND,
  PROGRESS_BAR,
  TEXT_INPUT,
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

typedef struct {
  char text[300];
  size_t align;
  int8_t text_color;
  int8_t bg_color;
  int8_t selected;
  size_t min_width;
  size_t min_height;
} TextInput;


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
}* ctermui_component;

ctermui_component ctermui_new_button(char* id,
                                     char* text,
                                     size_t align,
                                     int8_t text_color,
                                     int8_t bg_color);
ctermui_component ctermui_new_text(
  char* id, char* text, int8_t color, int8_t bg_color, size_t align);
ctermui_component ctermui_new_frame(char* id,
                                    int8_t color,
                                    int8_t bg_color);
ctermui_component ctermui_new_solid_background(char* id,
                                               int8_t color,
                                               size_t width,
                                               size_t height);

ctermui_component ctermui_new_soft_background(char* id,
                                              int8_t color);

ctermui_component ctermui_new_progress_bar(char* id,
                                           int8_t bar_color,
                                           int8_t bg_color,
                                           size_t max,
                                           size_t progress,
                                           char* text,
                                           int8_t text_color,
                                           size_t orientation);
ctermui_component ctermui_new_custom_component(
  char* id,
  void (*draw)(ctermui_screen_t s, ctermui_component c),
  void (*calculate_absolute_position)(ctermui_component c,
                                      size_t parent_x,
                                      size_t parent_y,
                                      size_t parent_width,
                                      size_t parent_height),
  void* core_component
  );
void ctermui_component_draw_button(ctermui_screen_t s,
                                   ctermui_component c);
void ctermui_component_draw_label(ctermui_screen_t s,
                                  ctermui_component c);
void ctermui_component_draw_frame(ctermui_screen_t s,
                                  ctermui_component c);
void ctermui_component_draw_solid_background(
  ctermui_screen_t s, ctermui_component c);

ctermui_component ctermui_new_text_input(
  char* id,
  int8_t text_color,
  int8_t bg_color,
  size_t min_width,
  size_t min_height,
  ctermui_screen_keyboard_events_t events);
void ctermui_component_draw_soft_background(
  ctermui_screen_t s, ctermui_component c);
#endif  // CTERMUI_COMPONENT_H
