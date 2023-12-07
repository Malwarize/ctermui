#ifndef CTERMUI_SCREEN_H
#define CTERMUI_SCREEN_H
#include <stdint.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include "ctermui_events.h"
#include "ctermui_widget.h"
#include <unistd.h>
#include "ctermui_pencil.h"
typedef struct winsize winsize;
typedef struct ctermui_screen{
    char*** buffer; 
    uint32_t width;
    uint32_t height;
    ctermui_widget root;
    ctermui_screen_keyboard_events_t keyboard_events;

    uint32_t loop_count;
    uint8_t loop_running;
    int loop_stop;
    int loop_idle;
}* ctermui_screen_t;


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
void ctermui_screen_clear(ctermui_screen_t s);
void ctermui_screen_free(ctermui_screen_t s);   
void ctermui_screen_set_widget_root(ctermui_screen_t s, ctermui_widget root);
void ctermui_screen_refresh_widgets(ctermui_screen_t s);
void ctermui_on_keybord_listener(ctermui_screen_t* s);
void ctermui_screen_clean_term();
void ctermui_restore_cursor();
void ctermui_screen_draw_all_components_of_widget(ctermui_screen_t s, ctermui_widget w);
void ctermui_screen_redraw_all_components_of_widget(ctermui_screen_t s, ctermui_widget new_w, int old_x, int old_y, int old_width, int old_height);
void ctermui_screen_display_widget(ctermui_screen_t s, ctermui_widget w);
void ctermui_screen_refresh_widget(ctermui_screen_t s, ctermui_widget w, uint32_t old_x, uint32_t old_y, uint32_t old_width, uint32_t old_height);

// screen loop control
void ctermui_screen_loop_start(ctermui_screen_t s, void (*periodic_func)(ctermui_screen_t), int every);
void ctermui_screen_loop_shutdown(ctermui_screen_t s);
void ctermui_screen_loop_pause(ctermui_screen_t s);
void ctermui_screen_loop_resume(ctermui_screen_t s);
int ctermui_kbhit();
ctermui_component ctermui_new_custom_component(char* id, void (*draw)(ctermui_screen_t s, ctermui_component c));

#endif // CTERMUI_SCREEN_H