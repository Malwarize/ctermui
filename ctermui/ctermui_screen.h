#ifndef CTERMUI_SCREEN_H
#define CTERMUI_SCREEN_H
#include <stdint.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "ctermui_widget.h"
#include "ctermui_events.h"
#include <termios.h>
typedef struct winsize winsize;
typedef struct ctermui_screen{
    char*** buffer; // 3D array of chars + color
    uint32_t width;
    uint32_t height;
    ctermui_widget root;
    ctermui_screen_keyboard_events_t keyboard_events;
}*ctermui_screen_t;

enum ctermui_color{
    CTERMUI_EMPTY = -1,
    CTERMUI_BLACK = 0,
    CTERMUI_RED = 1,
    CTERMUI_GREEN = 2,
    CTERMUI_YELLOW = 3,
    CTERMUI_BLUE = 4,
    CTERMUI_MAGENTA = 5,
    CTERMUI_CYAN = 6,
    CTERMUI_WHITE = 7,
    CTERMUI_BRIGHT_BLACK = 8,
    CTERMUI_BRIGHT_RED = 9,
    CTERMUI_BRIGHT_GREEN = 10,
    CTERMUI_BRIGHT_YELLOW = 11,
    CTERMUI_BRIGHT_BLUE = 12,
    CTERMUI_BRIGHT_MAGENTA = 13,
    CTERMUI_BRIGHT_CYAN = 14,
    CTERMUI_BRIGHT_WHITE = 15
};

#define CTERMUI_TOP_LEFT_CORNER '+'
#define CTERMUI_BOTTOM_LEFT_CORNER '+'
#define CTERMUI_TOP_RIGHT_CORNER '+'
#define CTERMUI_BOTTOM_RIGHT_CORNER '+'
#define CTERMUI_HORIZONTAL_LINE '-'
#define CTERMUI_VERTICAL_LINE '|'


ctermui_screen_t ctermui_screen_new();
void ctermui_screen_display(ctermui_screen_t s);
void ctermui_screen_test(ctermui_screen_t s);
void ctermui_screen_clear(ctermui_screen_t s);
void ctermui_screen_free(ctermui_screen_t s);   
void ctermui_start(ctermui_screen_t s);
void ctermui_screen_set_widget_root(ctermui_screen_t s, ctermui_widget root);
int ctermui_screen_draw_component_text(ctermui_screen_t s,  ctermui_component c);
int ctermui_screen_draw_frame(ctermui_screen_t s, ctermui_component c);
int ctermui_screen_draw_background(ctermui_screen_t s, ctermui_component c);
void ctermui_screen_refresh_widgets(ctermui_screen_t s);
int ctermui_screen_draw_char(ctermui_screen_t s, int x, int y, char c, int fg_color, int bg_color);
void ctermui_screen_refresh(ctermui_screen_t s);
void ctermui_on_keybord_listener(ctermui_screen_t* s);
void __ctermui_screen_clean_term();
#endif // CTERMUI_SCREEN_H