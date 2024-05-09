/*
 * XORbit 5/8/2024
 */
#ifndef CTERMUI_SCREEN_H
#define CTERMUI_SCREEN_H

#include "ctermui_events.h"
#include "ctermui_layout.h"
#include "ctermui_pencil.h"
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

typedef struct winsize winsize;

/**
 * @brief This struct represents the screen
 * it contains the screen buffer, the screen width and height, the root layout of the screen
 * the keyboard events and the loop state
 * to initialize the screen use `ctermui_screen_init`
 * @relatedalso ctermui_screen_init ctermui_screen_keyboard_events_new ctermui_screen_display ctermui_screen_clear ctermui_screen_set_layout_root ctermui_screen_refresh_layouts ctermui_screen_draw_all_components_of_layout ctermui_screen_redraw_all_components_of_layout ctermui_screen_display_layout ctermui_screen_refresh_layout ctermui_screen_loop_start ctermui_screen_loop_shutdown ctermui_screen_loop_pause ctermui_screen_loop_resume
 */
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
  CTERMUI_BLACK,
  CTERMUI_RED,
  CTERMUI_GREEN,
  CTERMUI_YELLOW,
  CTERMUI_BLUE,
  CTERMUI_MAGENTA,
  CTERMUI_CYAN,
  CTERMUI_WHITE,
  CTERMUI_BRIGHT_BLACK,
  CTERMUI_BRIGHT_RED,
  CTERMUI_BRIGHT_GREEN,
  CTERMUI_BRIGHT_YELLOW,
  CTERMUI_BRIGHT_BLUE,
  CTERMUI_BRIGHT_MAGENTA,
  CTERMUI_BRIGHT_CYAN,
  CTERMUI_BRIGHT_WHITE
};

/**
 * @brief This function initializes the screen
 * This function initializes the screen by allocating the screen buffer of size of the current terminal size
 * its use `get_term_size` to get the terminal size
 * @code
 * ctermui_screen_t ctermui_screen_init() {
     ctermui_screen_t screen =
             malloc(sizeof(struct ctermui_screen));
     if (!screen) {
         fprintf(
                 stderr,
                 "Error: could not allocate memory for screen\n"
         );
         exit(EXIT_FAILURE);
     }
     winsize w = get_term_size();
     screen->width = w.ws_col;
     screen->height = w.ws_row;

     screen->keyboard_events =
             ctermui_screen_keyboard_events_new();
     // loop state
     screen->loop_count = 0;
     screen->loop_running = 0;
     screen->loop_stop = 0;
     screen->loop_idle = 0;
     allocate_screen_buffer(screen);
     return screen;
  }
 * @return ctermui_screen_t
 */
ctermui_screen_t ctermui_screen_init();

void ctermui_screen_display(ctermui_screen_t s);

/**
 * @brief This function clears the screen buffer
 * it fills the screen buffer with empty cells
 * @code
 * ctermui_pencil_draw_char(
                    s->buffer,
                    x,
                    y,
                    EMPTY_CHAR,
                    CTERMUI_WHITE,
                    CTERMUI_EMPTY,
                    0
            );
 @endcode
 * @param s screen
 */
void ctermui_screen_clear(ctermui_screen_t s);

/**
 * @brief This function sets the root layout of the screen
 * @param s
 * @param root
 */
void ctermui_screen_set_layout_root(ctermui_screen_t s, ctermui_layout_t root);

/**
 * @brief This function refreshes all the layouts in the screen
 * This function clear the screen buffer and redraw all the layouts after recalculating  their dimensions
 * @relatedalso ctermui_screen_refresh_layout
 * @param s screen
 * @code
 * void ctermui_screen_refresh_layouts(ctermui_screen_t s) {
 *  ctermui_screen_clean_term();
 *  ctermui_screen_clear(s);
 *  ctermui_calculate_abs_position(s->root);
 *  ctermui_screen_draw_all_components_of_layout(s, s->root);
 *  ctermui_screen_display(s);
 * }
 * @endcode
 */
void ctermui_screen_refresh_layouts(ctermui_screen_t s);

void ctermui_screen_clean_term();

void ctermui_restore_cursor();

void ctermui_screen_draw_all_components_of_layout(ctermui_screen_t s,
                                                  ctermui_layout_t l);

/**
 * @brief This function redraws all the components of a layout
 * This function is responsible for redrawing all the components of a layout
 * it clear the old components and redraw the new components in the screen buffer
 * it takes `old_x`, `old_y`, `old_width`, `old_height` to clear the old components
 * and `new_l` to draw the new components
 * @param s screen
 * @param new_l
 * @param old_x
 * @param old_y
 * @param old_width
 * @param old_height
 */
void ctermui_screen_redraw_all_components_of_layout(ctermui_screen_t s,
                                                    ctermui_layout_t new_l,
                                                    size_t old_x, size_t old_y,
                                                    size_t old_width,
                                                    size_t old_height);

/**
 * @brief This function prints a part of the screen buffer to the terminal that consist of the layout `l`
 * @param s screen
 * @param l layout
 */
void ctermui_screen_display_layout(ctermui_screen_t s, ctermui_layout_t l);

/**
 *
 *  @brief This function refreshes a layout
 *  This function is responsible for refreshing the layout `l` and all its children
 *  the refresh is done by calculating the new layout dimensions and position
 *  then redrawing the layout
 *  on the screen buffer
 * @param s
 * @param l
 * @relatedalso  ctermui_screen_refresh_layouts
 */
void ctermui_screen_refresh_layout(ctermui_screen_t s, ctermui_layout_t l);

/**
 * @brief  This function starts the screen behavior loop, this loop is
 * responsible for refreshing the screen every `every` milliseconds
 * @param s screen
 * @param periodic_func func function to be called every `every` milliseconds
 * @param every  time in milliseconds
 */
void ctermui_screen_loop_start(ctermui_screen_t s,
                               void (*periodic_func)(ctermui_screen_t *),
                               size_t every);

/**
 * @brief This function stops the screen behavior loop
 * @param s screen
 * @relatedalso ctermui_screen_loop_resume ctermui_screen_loop_pause
 */
void ctermui_screen_loop_shutdown(ctermui_screen_t s);

/**
 * @brief This function pauses the screen behavior loop
 * @relatedalso ctermui_screen_loop_resume ctermui_screen_loop_shutdown
 * @param s
 */
void ctermui_screen_loop_pause(ctermui_screen_t s);
/**
 * @brief This function resumes the screen behavior loop
 * @relatedalso ctermui_screen_loop_pause ctermui_screen_loop_shutdown
 * @param s
 */
void ctermui_screen_loop_resume(ctermui_screen_t s);

#endif // CTERMUI_SCREEN_H
