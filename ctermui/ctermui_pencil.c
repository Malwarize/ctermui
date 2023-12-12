#include "ctermui_pencil.h"
#include <stdio.h>
#include <stdlib.h>

int ctermui_pencil_draw_char(screen_buffer b,
                             int x,
                             int y,
                             char c,
                             int fg_color,
                             int bg_color)
{
  b[x][y][0] = c;
  b[x][y][1] = fg_color;
  b[x][y][2] = bg_color;
  return 0;
}
int ctermui_pencil_draw_line(screen_buffer b,
                             int orientaion,
                             int x,
                             int y,
                             int length,
                             char c,
                             int color,
                             int bg_color)
{
  if (orientaion == 0) {
    for (int i = 0; i < length; i++) {
      ctermui_pencil_draw_char(
        b, x + i, y, c, color, bg_color);
    }
  }
  else if (orientaion == 1) {
    for (int i = 0; i < length; i++) {
      ctermui_pencil_draw_char(
        b, x, y + i, c, color, bg_color);
    }
  }
  else {
    fprintf(
      stderr,
      "ctermui_pencil_draw_line: invalid orientation\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}
int ctermui_pencil_draw_rect(screen_buffer b,
                             int x,
                             int y,
                             int width,
                             int height,
                             int color,
                             int bg_color)
{
  ctermui_pencil_draw_char(
    b, x, y, CTERMUI_TOP_LEFT_CORNER, color, bg_color);
  ctermui_pencil_draw_char(b,
                           x + width,
                           y,
                           CTERMUI_TOP_RIGHT_CORNER,
                           color,
                           bg_color);
  ctermui_pencil_draw_char(b,
                           x,
                           y + height,
                           CTERMUI_BOTTOM_LEFT_CORNER,
                           color,
                           bg_color);
  ctermui_pencil_draw_char(b,
                           x + width,
                           y + height,
                           CTERMUI_BOTTOM_RIGHT_CORNER,
                           color,
                           bg_color);
  ctermui_pencil_draw_line(b,
                           0,
                           x + 1,
                           y,
                           width - 1,
                           CTERMUI_HORIZONTAL_LINE,
                           color,
                           bg_color);
  ctermui_pencil_draw_line(b,
                           0,
                           x + 1,
                           y + height,
                           width - 1,
                           CTERMUI_HORIZONTAL_LINE,
                           color,
                           bg_color);
  ctermui_pencil_draw_line(b,
                           1,
                           x,
                           y + 1,
                           height - 1,
                           CTERMUI_VERTICAL_LINE,
                           color,
                           bg_color);
  ctermui_pencil_draw_line(b,
                           1,
                           x + width,
                           y + 1,
                           height - 1,
                           CTERMUI_VERTICAL_LINE,
                           color,
                           bg_color);
  return 0;
}
int ctermui_pencil_draw_text(screen_buffer b,
                             int x,
                             int y,
                             char* text,
                             int color,
                             int bg_color)
{
  int i = 0;
  while (text[i] != '\0') {
    ctermui_pencil_draw_char(
      b, x + i, y, text[i], color, bg_color);
    i++;
  }
  return 0;
}
int ctermui_pencil_solid_background(screen_buffer b,
                                    int x,
                                    int y,
                                    int width,
                                    int height,
                                    int color)
{
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      ctermui_pencil_draw_char(
        b, x + j, y + i, b[x][y][0], b[x][y][1], color);
    }
  }
  return 0;
}

int ctermui_pencil_bucket(screen_buffer b,
                          int x,
                          int y,
                          int width,
                          int height,
                          int color)
{
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (b[x + j][y + i][2] == -1) {
        ctermui_pencil_draw_char(
          b, x + j, y + i, b[x][y][0], b[x][y][1], color);
      }
    }
  }
  return 0;
}
