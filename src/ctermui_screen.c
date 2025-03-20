#include "ctermui_screen.h"
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <unistd.h>

void sleep_microseconds(long microseconds) {
  struct timespec req;
  req.tv_sec = microseconds / 1000000; // Convert to seconds
  req.tv_nsec =
      (microseconds % 1000000) * 1000; // Convert remainder to nanoseconds
  nanosleep(&req, NULL);
}

winsize get_term_size() {
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w;
}

const char EMPTY_CHAR = ' ';

void ctermui_screen_clean_term() {
  printf("\033[?25l");
  system("clear");
}

void ctermui_restore_cursor() { printf("\033[?25h"); }

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

void ctermui_screen_clear(ctermui_screen_t s) {
  for (uint32_t x = 0; x < s->width; ++x) {
    for (uint32_t y = 0; y < s->height; ++y) {
      ctermui_pencil_draw_char(s->buffer, x, y, EMPTY_CHAR, CTERMUI_WHITE,
                               CTERMUI_EMPTY, 0);
    }
  }
}

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

void ctermui_screen_display(ctermui_screen_t s) {
  for (size_t i = 0; i < s->height; i++) {
    for (size_t j = 0; j < s->width; j++) {
      ctermui_display_cell(s->buffer[j][i]);
    }
  }
}

void ctermui_sigint_handler(size_t sig, void *data) {
  ctermui_screen_t s = (ctermui_screen_t)data;
  ctermui_restore_cursor();
  exit(0);
}

void ctermui_screen_set_layout_root(ctermui_screen_t s, ctermui_layout_t root) {
  s->root = root;
}

void ctermui_screen_draw_component(ctermui_screen_t s, ctermui_component_t c) {
  c->draw(s, c);
}

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

void ctermui_screen_redraw_all_components_of_layout(ctermui_screen_t s,
                                                    ctermui_layout_t new_l,
                                                    size_t old_x, size_t old_y,
                                                    size_t old_width,
                                                    size_t old_height) {
  ctermui_screen_clear_part(s, old_x, old_y, old_width, old_height);
  ctermui_screen_draw_all_components_of_layout(s, new_l);
}

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

void ctermui_screen_refresh_layouts(ctermui_screen_t s) {
  ctermui_screen_clean_term();
  ctermui_screen_clear(s);
  ctermui_calculate_abs_position(s->root);
  ctermui_screen_draw_all_components_of_layout(s, s->root);
  ctermui_screen_display(s);
}

void ctermui_screen_on_resize(ctermui_screen_t *sp) {
  ctermui_screen_free_buffer(*sp);
  (*sp)->width = get_term_size().ws_col;
  (*sp)->height = get_term_size().ws_row;
  allocate_screen_buffer(*sp);
  (*sp)->root->width = (*sp)->width;
  (*sp)->root->height = (*sp)->height;
  ctermui_screen_refresh_layouts(*sp);
}

int ctermui_on_resize_listener(ctermui_screen_t *s) {
  winsize w = get_term_size();
  if (w.ws_col != (*s)->width || w.ws_row != (*s)->height) {
    ctermui_screen_on_resize(s);
    return 1;
  }
  return 0;
}

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

void ctermui_screen_display_layout(ctermui_screen_t s, ctermui_layout_t l) {
  ctermui_screen_display_part(s, l->x, l->y, l->width, l->height);
}

void ctermui_screen_loop_shutdown(ctermui_screen_t s) { s->loop_stop = 1; }

void ctermui_screen_loop_pause(ctermui_screen_t s) { s->loop_idle = 1; }

void ctermui_screen_loop_resume(ctermui_screen_t s) { s->loop_idle = 0; }

struct termios orig_termios;

void ctermui_disable_raw_mode() {
  // Restore the original terminal settings
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void exit_trigger(void) {
  // free the buffer
  // restore the cursor
  ctermui_disable_raw_mode();
}

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
 * Start the screen loop
 * @param s screen
 * @param periodic_func periodic function
 * @param every time in microseconds
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
