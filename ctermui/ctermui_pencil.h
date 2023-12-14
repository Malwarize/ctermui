#ifndef CTERMUI_PENCIL_H
#define CTERMUI_PENCIL_H
#include <stddef.h>
typedef char*** screen_buffer;
#define CTERMUI_TOP_LEFT_CORNER     '+'
#define CTERMUI_BOTTOM_LEFT_CORNER  '+'
#define CTERMUI_TOP_RIGHT_CORNER    '+'
#define CTERMUI_BOTTOM_RIGHT_CORNER '+'
#define CTERMUI_HORIZONTAL_LINE     '-'
#define CTERMUI_VERTICAL_LINE       '|'

int ctermui_pencil_draw_char(screen_buffer b,
                             size_t x,
                             size_t y,
                             char c,
                             size_t fg_color,
                             size_t bg_color);
int ctermui_pencil_draw_line(screen_buffer b,
                             size_t orientation,
                             size_t x,
                             size_t y,
                             size_t length,
                             char c,
                             size_t color,
                             size_t bg_color);
int ctermui_pencil_draw_rect(screen_buffer b,
                             size_t x,
                             size_t y,
                             size_t width,
                             size_t height,
                             size_t color,
                             size_t bg_color);
int ctermui_pencil_draw_text(screen_buffer b,
                             size_t x,
                             size_t y,
                             char* text,
                             size_t color,
                             size_t bg_color);
int ctermui_pencil_solid_background(screen_buffer b,
                                    size_t x,
                                    size_t y,
                                    size_t width,
                                    size_t height,
                                    size_t color);
int ctermui_pencil_bucket(screen_buffer b,
                          size_t x,
                          size_t y,
                          size_t width,
                          size_t height,
                          size_t color);
#endif
