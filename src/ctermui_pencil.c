#include "ctermui_pencil.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Draw a single character at a given position in the screen buffer.
 *
 * @param b The screen buffer.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @param c The character to draw.
 * @param fg_color Foreground color.
 * @param bg_color Background color.
 * @param flag Additional flag for cell state.
 * @return 0 on success.
 */
int ctermui_pencil_draw_char(
        screen_buffer b,
        size_t x,
        size_t y,
        char c,
        int8_t fg_color,
        int8_t bg_color,
        uint8_t flag
) {
    b[x][y]->character = c;
    b[x][y]->foreground_color = fg_color;
    b[x][y]->background_color = bg_color;
    return 0;
}

/**
 * @brief Draw a horizontal or vertical line in the screen buffer.
 *
 * @param b The screen buffer.
 * @param orientation 0 for horizontal, 1 for vertical.
 * @param x Starting x-coordinate.
 * @param y Starting y-coordinate.
 * @param length Length of the line.
 * @param c Character to use for the line.
 * @param color Foreground color.
 * @param bg_color Background color.
 * @return 0 on success. Exits on invalid orientation.
 */
int ctermui_pencil_draw_line(
        screen_buffer b,
        size_t orientation,
        size_t x,
        size_t y,
        size_t length,
        char c,
        int8_t color,
        int8_t bg_color
) {
    if (orientation == 0) {
        for (size_t i = 0; i < length; i++) {
            ctermui_pencil_draw_char(
                    b, x + i, y, c, color, bg_color, 0
            );
        }
    } else if (orientation == 1) {
        for (size_t i = 0; i < length; i++) {
            ctermui_pencil_draw_char(
                    b, x, y + i, c, color, bg_color, 0
            );
        }
    } else {
        fprintf(
                stderr,
                "ctermui_pencil_draw_line: invalid orientation\n"
        );
        exit(EXIT_FAILURE);
    }
    return 0;
}

/**
 * @brief Draw a line between two points in the screen buffer (Bresenham's algorithm).
 *
 * @param b The screen buffer.
 * @param x_1 Starting x-coordinate.
 * @param y_1 Starting y-coordinate.
 * @param x_2 Ending x-coordinate.
 * @param y_2 Ending y-coordinate.
 * @param color Foreground color.
 * @param bg_color Background color.
 * @param c Character to use for the line.
 * @return 0 on success.
 */
int ctermui_pencil_draw_line_from_points(
        screen_buffer b,
        size_t x_1,
        size_t y_1,
        size_t x_2,
        size_t y_2,
        int8_t color,
        int8_t bg_color,
        char c
) {
    int dx = abs((int) x_2 - (int) x_1);
    int dy = -abs((int) y_2 - (int) y_1);
    int sx = x_1 < x_2 ? 1 : -1;
    int sy = y_1 < y_2 ? 1 : -1;
    int err = dx + dy;

    while (x_1 != x_2 || y_1 != y_2) {
        ctermui_pencil_draw_char(b, x_1, y_1, c, color, bg_color, 0);

        int e2 = 2 * err;

        if (e2 >= dy) {
            err += dy;
            x_1 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y_1 += sy;
        }
    }

    return 0;
}

/**
 * @brief Draw a rectangle (box) in the screen buffer.
 *
 * @param b The screen buffer.
 * @param x Top-left x-coordinate.
 * @param y Top-left y-coordinate.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Foreground color.
 * @param bg_color Background color.
 * @return 0 on success.
 */
int ctermui_pencil_draw_rect(
        screen_buffer b,
        size_t x,
        size_t y,
        size_t width,
        size_t height,
        int8_t color,
        int8_t bg_color
) {
    ctermui_pencil_draw_char(
            b, x, y, CTERMUI_TOP_LEFT_CORNER, color, bg_color, 0
    );
    ctermui_pencil_draw_char(
            b,
            x + width,
            y,
            CTERMUI_TOP_RIGHT_CORNER,
            color,
            bg_color,
            0
    );
    ctermui_pencil_draw_char(
            b,
            x,
            y + height,
            CTERMUI_BOTTOM_LEFT_CORNER,
            color,
            bg_color,
            0
    );
    ctermui_pencil_draw_char(
            b,
            x + width,
            y + height,
            CTERMUI_BOTTOM_RIGHT_CORNER,
            color,
            bg_color,
            0
    );
    ctermui_pencil_draw_line(
            b,
            0,
            x + 1,
            y,
            width - 1,
            CTERMUI_HORIZONTAL_LINE,
            color,
            bg_color
    );
    ctermui_pencil_draw_line(
            b,
            0,
            x + 1,
            y + height,
            width - 1,
            CTERMUI_HORIZONTAL_LINE,
            color,
            bg_color
    );
    ctermui_pencil_draw_line(
            b,
            1,
            x,
            y + 1,
            height - 1,
            CTERMUI_VERTICAL_LINE,
            color,
            bg_color
    );
    ctermui_pencil_draw_line(
            b,
            1,
            x + width,
            y + 1,
            height - 1,
            CTERMUI_VERTICAL_LINE,
            color,
            bg_color
    );
    return 0;
}

/**
 * @brief Draw a string of text at a given position in the screen buffer.
 *
 * @param b The screen buffer.
 * @param x Starting x-coordinate.
 * @param y Starting y-coordinate.
 * @param text The text string to draw.
 * @param color Foreground color.
 * @param bg_color Background color.
 * @return 0 on success.
 */
int ctermui_pencil_draw_text(
        screen_buffer b,
        size_t x,
        size_t y,
        const char *text,
        int8_t color,
        int8_t bg_color
) {
    size_t i = 0;
    //convert char to string
    while (text[i] != '\0') {
        ctermui_pencil_draw_char(
                b, x + i, y, text[i], color, bg_color, 0
        );
        i++;
    }
    return 0;
}

/**
 * @brief Fill a rectangular area with a solid background color.
 *
 * @param b The screen buffer.
 * @param x Top-left x-coordinate.
 * @param y Top-left y-coordinate.
 * @param width Width of the area.
 * @param height Height of the area.
 * @param color Background color to fill.
 * @return 0 on success.
 */
int ctermui_pencil_solid_background(
        screen_buffer b,
        size_t x,
        size_t y,
        size_t width,
        size_t height,
        int8_t color
) {
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            ctermui_pencil_draw_char(
                    b,
                    x + j,
                    y + i,
                    b[x][y]->character,
                    b[x][y]->foreground_color,
                    color,
                    0
            );
        }
    }
    return 0;
}

/**
 * @brief Fill a rectangular area with a background color only if the cell is empty.
 *
 * @param b The screen buffer.
 * @param x Top-left x-coordinate.
 * @param y Top-left y-coordinate.
 * @param width Width of the area.
 * @param height Height of the area.
 * @param color Background color to fill.
 * @return 0 on success.
 */
int ctermui_pencil_bucket(
        screen_buffer b,
        size_t x,
        size_t y,
        size_t width,
        size_t height,
        int8_t color
) {
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (b[x + j][y + i]->background_color == -1) {
                ctermui_pencil_draw_char(
                        b,
                        x + j,
                        y + i,
                        b[x][y]->character,
                        b[x][y]->foreground_color,
                        color,
                        0
                );
            }
        }
    }
    return 0;
}

