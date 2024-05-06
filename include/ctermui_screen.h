#ifndef CTERMUI_SCREEN_H
#define CTERMUI_SCREEN_H

#include "ctermui_events.h"
#include "ctermui_layout.h"
#include "ctermui_pencil.h"
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


typedef struct winsize winsize;


typedef struct ctermui_screen {
    ctermui_screen_cell_t **buffer;
    uint32_t width;
    uint32_t height;
    ctermui_layout_t root;
    ctermui_screen_keyboard_events_t keyboard_events;
    uint32_t loop_count;
    uint8_t loop_running;
    size_t loop_stop;
    size_t loop_idle;
} *ctermui_screen_t;


enum ctermui_color {
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


ctermui_screen_t ctermui_screen_init();

void ctermui_screen_display(ctermui_screen_t s);

void ctermui_screen_clear(ctermui_screen_t s);

void ctermui_screen_set_layout_root(
        ctermui_screen_t s,
        ctermui_layout_t root
);

void ctermui_screen_refresh_layouts(ctermui_screen_t s);

void ctermui_on_keyboard_listener(ctermui_screen_t *s);

void ctermui_screen_clean_term();

void ctermui_restore_cursor();

void ctermui_screen_draw_all_components_of_layout(
        ctermui_screen_t s, ctermui_layout_t w
);

void ctermui_screen_redraw_all_components_of_layout(
        ctermui_screen_t s,
        ctermui_layout_t new_w,
        size_t old_x,
        size_t old_y,
        size_t old_width,
        size_t old_height
);

void ctermui_screen_display_layout(
        ctermui_screen_t s,
        ctermui_layout_t w
);

void ctermui_screen_refresh_layout(
        ctermui_screen_t s,
        ctermui_layout_t w
);

void ctermui_screen_loop_start(
        ctermui_screen_t s,
        void (*periodic_func)(ctermui_screen_t *),
        size_t every
);

void ctermui_screen_loop_shutdown(ctermui_screen_t s);

void ctermui_screen_loop_pause(ctermui_screen_t s);

void ctermui_screen_loop_resume(ctermui_screen_t s);

int ctermui_kbhit();

#endif  // CTERMUI_SCREEN_H
