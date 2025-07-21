#include "ctermui_screen.h"
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <unistd.h>

/**
 * @brief Sleep for a specified number of microseconds.
 *
 * @param microseconds Number of microseconds to sleep.
 */
void sleep_microseconds(long microseconds) {
  struct timespec req;
  req.tv_sec = microseconds / 1000000; // Convert to seconds
  req.tv_nsec =
      (microseconds % 1000000) * 1000; // Convert remainder to nanoseconds
  nanosleep(&req, NULL);
}

/**
 * @brief Get the current terminal size.
 *
 * @return winsize struct containing terminal width and height.
 */
winsize get_term_size() {
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w;
}

const char EMPTY_CHAR = ' ';

/**
 * @brief Hide the cursor and clear the terminal screen.
 */
void ctermui_screen_clean_term() {
  printf("\033[?25l");
  system("clear");
}

/**
 * @brief Restore the cursor visibility.
 */
void ctermui_restore_cursor() { printf("\033[?25h"); }

/**
 * @brief Allocate the screen buffer for drawing.
 *
 * @param s Pointer to the screen structure.
 * @note Exits on allocation failure. Caller must free with ctermui_screen_free_buffer.
 */
void allocate_screen_buffer(ctermui_screen_t s) {
  s->buffer = (ctermui_screen_cell_t **)malloc(s->width *
                                               sizeof(ctermui_screen_cell_t *));
  if (!s->buffer) {
    fprintf(stderr, "Error: could not allocate memory for screen buffer\n");
    exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < s->width; i++) {
    s->buffer[i] = (ctermui_screen_cell_t *)malloc(
        s->height * sizeof(ctermui_screen_cell_t));
    if (!s->buffer[i]) {
      fprintf(stderr, "Error: could not allocate memory for screen buffer\n");
      exit(EXIT_FAILURE);
    }
    for (uint32_t j = 0; j < s->height; j++) {
      s->buffer[i][j] =
          (ctermui_screen_cell_t)malloc(sizeof(struct ctermui_screen_cell));
      if (!s->buffer[i][j]) {
        fprintf(stderr, "Error: could not allocate memory for screen buffer\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

/**
 * @brief Initialize a new screen structure.
 *
 * Allocates and initializes the screen, sets up the buffer, and event handlers.
 *
 * @return Pointer to the newly allocated screen structure. Exits on allocation failure.
 * @note Caller must eventually free the screen and its buffer.
 */
ctermui_screen_t ctermui_screen_init() {
  ctermui_screen_t screen = malloc(sizeof(struct ctermui_screen));
  if (!screen) {
    fprintf(stderr, "Error: could not allocate memory for screen\n");
    exit(EXIT_FAILURE);
  }
  winsize w = get_term_size();
  screen->width = w.ws_col;
  screen->height = w.ws_row;

  screen->keyboard_events = ctermui_screen_keyboard_events_new();
  // loop state
  screen->loop_count = 0;
  screen->loop_running = 0;
  screen->loop_stop = 0;
  screen->loop_idle = 0;
  allocate_screen_buffer(screen);
  return screen;
}

/**
 * @brief Free the screen buffer memory.
 *
 * @param s Pointer to the screen structure.
 */
void ctermui_screen_free_buffer(ctermui_screen_t s) {
  for (size_t i = 0; i < s->width; i++) {
    for (size_t j = 0; j < s->height; j++) {
      free(s->buffer[i][j]);
    }
    free(s->buffer[i]);
  }
  free(s->buffer);
  s->buffer = NULL;
}

/**
 * @brief Clear the entire screen buffer.
 *
 * @param s Pointer to the screen structure.
 */
void ctermui_screen_clear(ctermui_screen_t s) {
  for (uint32_t x = 0; x < s->width; ++x) {
    for (uint32_t y = 0; y < s->height; ++y) {
      ctermui_pencil_draw_char(s->buffer, x, y, EMPTY_CHAR, CTERMUI_WHITE,
                               CTERMUI_EMPTY, 0);
    }
  }
}

/**
 * @brief Clear a rectangular part of the screen buffer.
 *
 * @param s Pointer to the screen structure.
 * @param x X coordinate of the top-left corner.
 * @param y Y coordinate of the top-left corner.
 * @param width Width of the area to clear.
 * @param height Height of the area to clear.
 */
void ctermui_screen_clear_part(ctermui_screen_t s, size_t x, size_t y,
                               size_t width, size_t height) {
  for (size_t i = y; i < y + height; i++) {
    for (size_t j = x; j < x + width; j++) {
      ctermui_pencil_draw_char(s->buffer, j, i, EMPTY_CHAR, CTERMUI_WHITE,
                               CTERMUI_EMPTY, 0);
    }
  }
  for (size_t i = x; i < x + width; i++) {
    ctermui_pencil_draw_char(s->buffer, i, y, EMPTY_CHAR, CTERMUI_WHITE,
                             CTERMUI_EMPTY, 0);
  }
}

#define ANSI_BG_FMT "\033[48;5;%dm"
#define ANSI_FG_FMT "\033[38;5;%dm"
#define ANSI_RESET_FMT "\033[0m"

/**
 * @brief Display a single cell on the terminal.
 *
 * @param c Pointer to the screen cell structure.
 */
void ctermui_display_cell(ctermui_screen_cell_t c) {
  if (c->flag == DEFAULT) {
    printf(ANSI_BG_FMT ANSI_FG_FMT "%c" ANSI_RESET_FMT, c->background_color,
           c->foreground_color, c->character);
  } else if (c->flag == START) {
    printf(ANSI_BG_FMT ANSI_FG_FMT "%c", c->background_color,
           c->foreground_color, c->character);
  } else if (c->flag == END) {
    printf("%c" ANSI_RESET_FMT, c->character);
  } else if (c->flag == MIDDLE) {
    printf("%c", c->character);
  } else {
    printf(ANSI_BG_FMT ANSI_FG_FMT "%c" ANSI_RESET_FMT, c->background_color,
           c->foreground_color, c->character);
  }
}

/**
 * @brief Display the entire screen buffer to the terminal.
 *
 * @param s Pointer to the screen structure.
 */
void ctermui_screen_display(ctermui_screen_t s) {
  for (size_t i = 0; i < s->height; i++) {
    for (size_t j = 0; j < s->width; j++) {
      ctermui_display_cell(s->buffer[j][i]);
    }
  }
}

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 *
 * @param sig Signal number.
 * @param data Pointer to the screen structure.
 */
void ctermui_sigint_handler(size_t sig, void *data) {
  ctermui_screen_t s = (ctermui_screen_t)data;
  ctermui_restore_cursor();
  exit(0);
}

/**
 * @brief Set the root layout for the screen.
 *
 * @param s Pointer to the screen structure.
 * @param root Pointer to the root layout structure.
 */
void ctermui_screen_set_layout_root(ctermui_screen_t s, ctermui_layout_t root) {
  s->root = root;
}

/**
 * @brief Draw a single component on the screen.
 *
 * @param s Pointer to the screen structure.
 * @param c Pointer to the component structure.
 */
void ctermui_screen_draw_component(ctermui_screen_t s, ctermui_component_t c) {
  c->draw(s, c);
}

/**
 * @brief Recursively draw all components of a layout and its children.
 *
 * @param s Pointer to the screen structure.
 * @param l Pointer to the layout structure.
 */
void ctermui_screen_draw_all_components_of_layout(ctermui_screen_t s,
                                                  ctermui_layout_t l) {
  size_t i = 0;
  for (i = 0; i < l->component_count; i++) {
    if (l->components[i] && l->components[i]->draw == NULL) {
      fprintf(stderr, "%s:%d: Error: component %s has no draw function\n",
              __FILE__, __LINE__, l->components[i]->id);
      exit(EXIT_FAILURE);
    }
    l->components[i]->draw(s, l->components[i]);
  }
  for (size_t j = 0; j < l->children_count; j++) {
    ctermui_screen_draw_all_components_of_layout(s, l->children[j]);
  }
}

/**
 * @brief Redraw all components of a layout after clearing a specified area.
 *
 * @param s Pointer to the screen structure.
 * @param new_l Pointer to the new layout structure.
 * @param old_x Old X coordinate.
 * @param old_y Old Y coordinate.
 * @param old_width Old width.
 * @param old_height Old height.
 */
void ctermui_screen_redraw_all_components_of_layout(ctermui_screen_t s,
                                                    ctermui_layout_t new_l,
                                                    size_t old_x, size_t old_y,
                                                    size_t old_width,
                                                    size_t old_height) {
  ctermui_screen_clear_part(s, old_x, old_y, old_width, old_height);
  ctermui_screen_draw_all_components_of_layout(s, new_l);
}

/**
 * @brief Refresh a specific layout, recalculating positions and redrawing.
 *
 * @param s Pointer to the screen structure.
 * @param l Pointer to the layout structure.
 */
void ctermui_screen_refresh_layout(ctermui_screen_t s, ctermui_layout_t l) {
  size_t old_x = l->x;
  size_t old_y = l->y;
  size_t old_width = l->width;
  size_t old_height = l->height;
  ctermui_calculate_abs_position(s->root);
  ctermui_screen_redraw_all_components_of_layout(s, l, old_x, old_y, old_width,
                                                 old_height);
  ctermui_screen_display_layout(s, l);
}

/**
 * @brief Refresh all layouts and redraw the entire screen.
 *
 * @param s Pointer to the screen structure.
 */
void ctermui_screen_refresh_layouts(ctermui_screen_t s) {
  ctermui_screen_clean_term();
  ctermui_screen_clear(s);
  ctermui_calculate_abs_position(s->root);
  ctermui_screen_draw_all_components_of_layout(s, s->root);
  ctermui_screen_display(s);
}

/**
 * @brief Handle terminal resize event and update screen and layouts.
 *
 * @param sp Pointer to the screen structure pointer.
 */
void ctermui_screen_on_resize(ctermui_screen_t *sp) {
  ctermui_screen_free_buffer(*sp);
  (*sp)->width = get_term_size().ws_col;
  (*sp)->height = get_term_size().ws_row;
  allocate_screen_buffer(*sp);
  (*sp)->root->width = (*sp)->width;
  (*sp)->root->height = (*sp)->height;
  ctermui_screen_refresh_layouts(*sp);
}

/**
 * @brief Listener for terminal resize events.
 *
 * @param s Pointer to the screen structure pointer.
 * @return 1 if resized, 0 otherwise.
 */
int ctermui_on_resize_listener(ctermui_screen_t *s) {
  winsize w = get_term_size();
  if (w.ws_col != (*s)->width || w.ws_row != (*s)->height) {
    ctermui_screen_on_resize(s);
    return 1;
  }
  return 0;
}

/**
 * @brief Check if a keyboard key has been hit (non-blocking).
 *
 * @return 1 if a key was hit, 0 otherwise.
 */
int ctermui_kbhit() {
  struct termios oldt, newt;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  size_t oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  int ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

/**
 * @brief Keyboard event listener, dispatches key events to registered handlers.
 *
 * @param s Pointer to the screen structure pointer.
 */
void ctermui_on_keyboard_listener(ctermui_screen_t *s) {
  if (ctermui_kbhit()) {
    char c = getchar();
    // arrows have 3 chars
    if (c == '\033') {
      // skip [
      getchar();
      c = (char)getchar();
      switch (c) {
      case 'A':
        c = CTERMUI_KEY_UP;
        break;
      case 'B':
        c = CTERMUI_KEY_DOWN;
        break;
      case 'C':
        c = CTERMUI_KEY_RIGHT;
        break;
      case 'D':
        c = CTERMUI_KEY_LEFT;
        break;
      case 'H':
        c = CTERMUI_KEY_ESC;
        break;
      case 'F':
        c = CTERMUI_KEY_ENTER;
        break;
      case 'Z':
        c = CTERMUI_KEY_TAB;
        break;
      default:;
      }
    }
    ctermui_screen_keyboard_events_handle((*s)->keyboard_events, c);
  }
}

/**
 * @brief Display a rectangular part of the screen buffer.
 *
 * @param s Pointer to the screen structure.
 * @param x X coordinate of the top-left corner.
 * @param y Y coordinate of the top-left corner.
 * @param width Width of the area to display.
 * @param height Height of the area to display.
 */
void ctermui_screen_display_part(ctermui_screen_t s, size_t x, size_t y,
                                 size_t width, size_t height) {
  // adjust the cursor position
  /* printf("\033[%zu;%zuH", y + 1, x + 1); */
  for (size_t i = y; i < y + height; i++) {
    printf("\033[%zu;%zuH", i + 1, x + 1);
    for (size_t j = x; j < x + width; j++) {
      ctermui_display_cell(s->buffer[j][i]);
    }
  }
}

/**
 * @brief Display a layout's area on the terminal.
 *
 * @param s Pointer to the screen structure.
 * @param l Pointer to the layout structure.
 */
void ctermui_screen_display_layout(ctermui_screen_t s, ctermui_layout_t l) {
  ctermui_screen_display_part(s, l->x, l->y, l->width, l->height);
}

/**
 * @brief Signal the screen loop to stop.
 *
 * @param s Pointer to the screen structure.
 */
void ctermui_screen_loop_shutdown(ctermui_screen_t s) { s->loop_stop = 1; }

/**
 * @brief Pause the screen loop.
 *
 * @param s Pointer to the screen structure.
 */
void ctermui_screen_loop_pause(ctermui_screen_t s) { s->loop_idle = 1; }

/**
 * @brief Resume the screen loop.
 *
 * @param s Pointer to the screen structure.
 */
void ctermui_screen_loop_resume(ctermui_screen_t s) { s->loop_idle = 0; }

struct termios orig_termios;

/**
 * @brief Restore the original terminal settings (disable raw mode).
 */
void ctermui_disable_raw_mode() {
  // Restore the original terminal settings
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/**
 * @brief Cleanup function to be called at exit, disables raw mode.
 */
void exit_trigger(void) {
  // free the buffer
  // restore the cursor
  ctermui_disable_raw_mode();
}

/**
 * @brief Enable raw mode for the terminal (disable echo and canonical mode).
 */
void ctermui_enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(ctermui_disable_raw_mode); // Ensure disableRawMode is called at exit

  struct termios raw = orig_termios;

  // Modify the settings for raw mode
  raw.c_lflag &= ~(ECHO | ICANON);

  // Apply the modified settings
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/**
 * @brief Start the main screen loop, handling periodic updates and events.
 *
 * @param s Pointer to the screen structure.
 * @param periodic_func Function pointer to be called periodically.
 * @param every Time interval in microseconds between periodic calls.
 */
void ctermui_screen_loop_start(ctermui_screen_t s,
                               void (*periodic_func)(ctermui_screen_t *),
                               size_t every) {
  ctermui_enable_raw_mode();
  signal(SIGINT, (void (*)(int))ctermui_sigint_handler);
  ctermui_screen_refresh_layouts(s);
  while (1) {
    if (s->loop_idle) {
      sleep_microseconds(every);
      continue;
    }

    if (ctermui_on_resize_listener(&s)) {
      continue;
    }

    if (periodic_func)
      periodic_func(&s);
    ctermui_on_keyboard_listener(&s);
    if (s->loop_stop) {
      ctermui_disable_raw_mode();
      ctermui_restore_cursor();
      break;
    }
    sleep_microseconds(every);
    s->loop_count++;
  }
}
