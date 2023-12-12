#ifndef CTERMUI_PENCIL_H
#define CTERMUI_PENCIL_H
typedef char*** screen_buffer;
#define CTERMUI_TOP_LEFT_CORNER     '+'
#define CTERMUI_BOTTOM_LEFT_CORNER  '+'
#define CTERMUI_TOP_RIGHT_CORNER    '+'
#define CTERMUI_BOTTOM_RIGHT_CORNER '+'
#define CTERMUI_HORIZONTAL_LINE     '-'
#define CTERMUI_VERTICAL_LINE       '|'

int ctermui_pencil_draw_char(screen_buffer b,
                             int x,
                             int y,
                             char c,
                             int fg_color,
                             int bg_color);
int ctermui_pencil_draw_line(screen_buffer b,
                             int orientaion,
                             int x,
                             int y,
                             int length,
                             char c,
                             int color,
                             int bg_color);
int ctermui_pencil_draw_rect(screen_buffer b,
                             int x,
                             int y,
                             int width,
                             int height,
                             int color,
                             int bg_color);
int ctermui_pencil_draw_text(screen_buffer b,
                             int x,
                             int y,
                             char* text,
                             int color,
                             int bg_color);
int ctermui_pencil_solid_background(screen_buffer b,
                                    int x,
                                    int y,
                                    int width,
                                    int height,
                                    int color);
int ctermui_pencil_bucket(screen_buffer b,
                          int x,
                          int y,
                          int width,
                          int height,
                          int color);
#endif
