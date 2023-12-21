#ifndef CTERMUI_PENCIL_H
#define CTERMUI_PENCIL_H
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <stddef.h>
#define CTERMUI_TOP_LEFT_CORNER     '+'
#define CTERMUI_BOTTOM_LEFT_CORNER  '+'
#define CTERMUI_TOP_RIGHT_CORNER    '+'
#define CTERMUI_BOTTOM_RIGHT_CORNER '+'
#define CTERMUI_HORIZONTAL_LINE '-'  // Unicode code point for '─'
#define CTERMUI_VERTICAL_LINE   '|'  // Unicode code point for '│'
#define CTERMUI_ASCENDING_LINE   '/'  // Unicode code point for '╱'
#define CTERMUI_DESCENDING_LINE '\\'  // Unicode code point for '╲'

#define CELL_CHARACTER_SIZE 50


enum ctermui_cell_flag{
  DEFAULT = 0,
  START,
  END,
  MIDDLE,
  SKIP_CELL,
};

typedef struct ctermui_screen_cell{
  char character;
  int8_t foreground_color;
  int8_t background_color;
  uint8_t flag;
}*ctermui_screen_cell_t;

enum LINE_ORIENTATION {
  CTERMUI_LINE_HORIZONTAL,
  CTERMUI_LINE_VERTICAL,
  CTEMRUI_LINE_DIAGONAL
};

typedef ctermui_screen_cell_t** screen_buffer;

int ctermui_pencil_draw_char(screen_buffer b,
                             size_t x,
                             size_t y,
                             char c,
                             int8_t fg_color,
                             int8_t bg_color,
                             uint8_t flag);
int ctermui_pencil_draw_line(screen_buffer b,
                             size_t orientation,
                             size_t x,
                             size_t y,
                             size_t length,
                             char c,
                             int8_t color,
                             int8_t bg_color);
int ctermui_pencil_draw_rect(screen_buffer b,
                             size_t x,
                             size_t y,
                             size_t width,
                             size_t height,
                             int8_t color,
                             int8_t bg_color);
int ctermui_pencil_draw_text(screen_buffer b,
                             size_t x,
                             size_t y,
                             const char* text,
                             int8_t color,
                             int8_t bg_color);
int ctermui_pencil_solid_background(screen_buffer b,
                                    size_t x,
                                    size_t y,
                                    size_t width,
                                    size_t height,
                                    int8_t color);
int ctermui_pencil_bucket(screen_buffer b,
                          size_t x,
                          size_t y,
                          size_t width,
                          size_t height,
                          int8_t color);

int ctermui_pencil_draw_line_with_points(screen_buffer b,
                              size_t x_1,
                              size_t y_1,
                              size_t x_2,
                              size_t y_2,
                              int8_t color,
                              int8_t bg_color,
                              char c
                              );
#endif
