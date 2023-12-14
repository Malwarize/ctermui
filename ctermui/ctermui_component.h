#ifndef CTERMUI_COMPONENT_H
#define CTERMUI_COMPONENT_H
typedef struct ctermui_screen* ctermui_screen_t;
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
  size_t color;
  size_t bg_color;

  size_t align;
} Text;

typedef struct {
  size_t color;
  size_t bg_color;

} Frame;

typedef struct {
  size_t color;
} SoftBackground;

typedef struct {
  size_t color;
} SolidBackground;

typedef struct {
  char text[100];
  size_t align;
  size_t text_color;
  size_t bg_color;
} Button;

#define PROGRESS_BAR_CHAR "█"

typedef struct {
  char symbol;  // default is █
  size_t bar_color;
  size_t bg_color;
  size_t max;
  size_t progress;
  char* text;
  size_t text_color;
  size_t orientation;  // 0 = horizontal, 1 = vertical
} ProgressBar;

typedef struct {
  char text[300];
  size_t align;
  size_t text_color;
  size_t bg_color;
  size_t selected;
  size_t min_width;
  size_t min_height;
} TextInput;

typedef struct {
  // TODO: implement custom component
} Custom;

typedef struct ctermui_component {
  char id[100];
  uint16_t type;
  size_t x;
  size_t y;
  size_t width;
  size_t height;

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
}* ctermui_component;

ctermui_component ctermui_new_button(char* id,
                                     char* text,
                                     size_t align,
                                     size_t text_color,
                                     size_t bg_color);
ctermui_component ctermui_new_text(
  char* id, char* text, size_t color, size_t bg_color, size_t align);
ctermui_component ctermui_new_frame(char* id,
                                    size_t color,
                                    size_t bg_color);
ctermui_component ctermui_new_solid_background(char* id,
                                               size_t color,
                                               size_t width,
                                               size_t height);

ctermui_component ctermui_new_soft_background(char* id,
                                              size_t color,
                                              size_t width);

ctermui_component ctermui_new_progress_bar(char* id,
                                           size_t bar_color,
                                           size_t bg_color,
                                           size_t max,
                                           size_t progress,
                                           char* text,
                                           size_t text_color,
                                           size_t orientation);
ctermui_component ctermui_new_custom_component(
  char* id,
  void (*draw)(ctermui_screen_t s, ctermui_component c),
  void (*calculate_absolute_position)(ctermui_component c,
                                      size_t parent_x,
                                      size_t parent_y,
                                      size_t parent_width,
                                      size_t parent_height));
void ctermui_component_draw_button(ctermui_screen_t s,
                                   ctermui_component c);
void ctermui_component_draw_label(ctermui_screen_t s,
                                  ctermui_component c);
void ctermui_component_draw_frame(ctermui_screen_t s,
                                  ctermui_component c);
void ctermui_component_draw_solid_background(
  ctermui_screen_t s, ctermui_component c);

/* 
 * @ brief: create a new text input component
*  
*  @ param id: id of the component
*  @ param text_color: color of the text
*  @ param bg_color: background color
*  @ param width: width of the component
*  @ param height: height of the component
*  @ return: a component
*/
#include "ctermui_events.h"
ctermui_component ctermui_new_text_input(
  char* id,
  size_t text_color,
  size_t bg_color,
  size_t min_width,
  size_t min_height,
  ctermui_screen_keyboard_events_t events);
void ctermui_component_draw_soft_background(
  ctermui_screen_t s, ctermui_component c);
#endif  // CTERMUI_COMPONENT_H
