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
  int color;
  int bg_color;

  int align;
} Text;

typedef struct {
  int color;
  int bg_color;

} Frame;

typedef struct {
  int color;
} SoftBackground;

typedef struct {
  int color;
} SolidBackground;

typedef struct {
  char text[100];
  int align;
  int text_color;
  int bg_color;
} Button;

#define PROGRESS_BAR_CHAR "█"

typedef struct {
  char symbol;  // default is █
  int bar_color;
  int bg_color;
  int max;
  int progress;
  char* text;
  int text_color;
  int orientation;  // 0 = horizontal, 1 = vertical
} ProgressBar;

typedef struct {
  char text[300];
  int align;
  int text_color;
  int bg_color;
  int selected;
} TextInput;

typedef struct {
  // TODO: implement custom component
} Custom;

typedef struct ctermui_component {
  char id[100];
  uint16_t type;
  int x;
  int y;
  int width;
  int height;

  int absolute_x;
  int absolute_y;
  int absolute_width;
  int absolute_height;
  void (*draw)(ctermui_screen_t s,
               struct ctermui_component* c);
  void (*calculate_absolute_position)(
    struct ctermui_component* c,
    int parent_x,
    int parent_y,
    int parent_width,
    int parent_height);
  void* core_component;
}* ctermui_component;

ctermui_component ctermui_new_button(char* id,
                                     char* text,
                                     int align,
                                     int text_color,
                                     int bg_color);
ctermui_component ctermui_new_text(
  char* id, char* text, int color, int bg_color, int align);
ctermui_component ctermui_new_frame(char* id,
                                    int color,
                                    int bg_color);
ctermui_component ctermui_new_solid_background(char* id,
                                               int color,
                                               int width,
                                               int height);

ctermui_component ctermui_new_soft_background(char* id,
                                              int color,
                                              int width,
                                              int height);

ctermui_component ctermui_new_progress_bar(char* id,
                                           int bar_color,
                                           int bg_color,
                                           int max,
                                           int progress,
                                           char* text,
                                           int text_color,
                                           int orientation);
ctermui_component ctermui_new_custom_component(
  char* id,
  void (*draw)(ctermui_screen_t s, ctermui_component c),
  void (*calculate_absolute_position)(ctermui_component c,
                                      int parent_x,
                                      int parent_y,
                                      int parent_width,
                                      int parent_height));
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
  int text_color,
  int bg_color,
  int width,
  int height,
  ctermui_screen_keyboard_events_t events);
void ctermui_component_draw_soft_background(
  ctermui_screen_t s, ctermui_component c);
#endif  // CTERMUI_COMPONENT_H
