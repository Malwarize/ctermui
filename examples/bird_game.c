#include "ctermui_component.h"
#include "ctermui_layout.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"
#include <stdlib.h>
#include <time.h>

#define BARRIER_WIDTH 20
#define BARRIER_WIDTH 20
#define GRAVITY 1
#define JUMP_VELOCITY -3
#define FALL_VELOCITY 1

typedef struct Bird {
  int x;
  int y;
  int y_velocity;
} *Bird;

enum Direction { UP, DOWN };

typedef struct Barrier {
  int x;
  int height;
  enum Direction direction;
} *Barrier;

typedef struct Game {
  Bird bird;
  Barrier *barriers;
  int bar_nb;
  int game_speed;
} *Game;

// draw
void draw_bird(ctermui_screen_t screen, Bird bird) {
  ctermui_pencil_draw_char(screen->buffer, bird->x, bird->y, 'B',
                           CTERMUI_BRIGHT_RED, CTERMUI_BRIGHT_RED, 0);
}

void draw_barrier(ctermui_screen_t screen, Barrier barrier) {
  for (int i = 0; i < barrier->height; i++) {
    // calculate barrier y position
    // if direction is up, draw from the top
    int y;
    if (barrier->direction == DOWN) {
      y = screen->height - barrier->height;
    } else {
      y = 0;
    }

    ctermui_pencil_draw_line(screen->buffer, CTERMUI_LINE_VERTICAL, barrier->x,
                             y, barrier->height, 'L', CTERMUI_BRIGHT_BLUE,
                             CTERMUI_BRIGHT_BLUE);
  }
}
void draw_barriers(ctermui_screen_t screen, Barrier barriers[], size_t nb) {
  for (int i = 0; i < nb; i++) {
    draw_barrier(screen, barriers[i]);
  }
}
void calculate_absolute_position(struct ctermui_component *c, size_t parent_x,
                                 size_t parent_y, size_t parent_width,
                                 size_t parent_height) {
  c->x = parent_x;
  c->y = parent_y;
  c->width = parent_width;
  c->height = parent_height;
}

void register_key_handlers(ctermui_screen_t s) {}

void draw_game(ctermui_screen_t screen, ctermui_component_t c) {
  Game game = c->core_component;
  draw_bird(screen, game->bird);
  draw_barriers(screen, game->barriers, game->bar_nb);
}

Barrier generate_barrier(size_t height, size_t width) {
  Barrier barrier = malloc(sizeof(struct Barrier));
  if (barrier == NULL) {
    return NULL;
  }
  barrier->x = width;
  barrier->height = rand() % height;
  barrier->direction = rand() % 2;
  return barrier;
}

ctermui_component_t new_game_component(size_t width, size_t height) {
  Game game;
  game = malloc(sizeof(struct Game));
  game->bird = malloc(sizeof(struct Bird));
  if (game == NULL || game->bird == NULL) {
    return NULL;
  }
  // bird in the middle
  game->bird->x = width / 6;
  game->bird->y = height / 2;
  game->bird->y_velocity = 0;
  // calculate barriers numbers per screen
  game->bar_nb = width / BARRIER_WIDTH;
  game->barriers = malloc(sizeof(struct Barrier) * game->bar_nb);
  game->game_speed = 2;
  // randomally generate barriers
  for (int i = 0; i < game->bar_nb; i++) {
    game->barriers[i] = malloc(sizeof(struct Barrier));
    if (i < game->bar_nb / 2) {
      // skip the first middle
      game->barriers[i]->height = 0;
    } else {
      game->barriers[i]->height = rand() % height - 1;
    }
    game->barriers[i]->x = i * BARRIER_WIDTH;
    game->barriers[i]->direction = rand() % 2;
  }
  return ctermui_new_custom_component("bird", game, draw_game,
                                      calculate_absolute_position);
}
// move barriers to the left
void move_barriers(Barrier barriers[], size_t nb, size_t width, size_t height) {
  for (int i = 0; i < nb; i++) {
    barriers[i]->x--;
    if (barriers[i]->x <= 0) {
      barriers[i]->x = width - 1;
      barriers[i]->height = rand() % height;
      barriers[i]->direction = rand() % 2;
    }
  }
}

void player_jump(Game game) { game->bird->y_velocity = JUMP_VELOCITY; }
void player_fall(Game game, size_t height) {
  game->bird->y_velocity += FALL_VELOCITY;
  game->bird->y += game->bird->y_velocity;
  if (game->bird->y < 0) {
    game->bird->y = 0;
    game->bird->y_velocity = 0; // Stop jumping when reaching top
  }

  // Ensure the bird does not fall below the bottom border
  if (game->bird->y + 1 >= height) {
    game->bird->y = height - 1;
    game->bird->y_velocity = 0; // Stop falling when reaching bottom
  }
}
// key handlers
void handle_space(void *data) {
  ctermui_screen_t s = data;
  ctermui_component_t game_c = s->root->components[0];
  Game game = game_c->core_component;

  player_jump(game);
}

int check_if_collision(Game game, size_t height) {
  Bird bird = game->bird;
  for (int i = 0; i < game->bar_nb; i++) {
    Barrier barrier = game->barriers[i];
    if (bird->x == barrier->x) {
      if (barrier->direction == UP && bird->y < barrier->height) {
        return 1;
      }
      if (barrier->direction == DOWN && bird->y > (height - barrier->height)) {
        return 1;
      }
    }
  }
  return 0;
}
// perdiodic
void periodic(ctermui_screen_t *sp) {
  ctermui_screen_t s = *sp;
  ctermui_component_t game_c = s->root->components[0];

  Game game = game_c->core_component;
  if (s->loop_count % 50 == 0) {
    player_fall(game, s->height);
  }
  if (s->loop_count % (100 / game->game_speed) == 0) {
    move_barriers(game->barriers, game->bar_nb, s->width, s->height);
  }

  if (s->loop_count % 10000 == 0) {
    game->game_speed += 1;
  }

  if (check_if_collision(game, s->height)) {
    ctermui_screen_loop_shutdown(s);
    exit(0);
  }
  ctermui_screen_refresh_layout(s, s->root);
}

int main() {
  // random seed
  srand(time(NULL));
  ctermui_screen_t screen = ctermui_screen_init();
  ctermui_screen_keyboard_events_register(screen->keyboard_events, ' ',
                                          handle_space, screen);
  ctermui_layout_t root = ctermui_layout_new_root(
      CTERMUI_HORIZONTAL, screen->width, screen->height);

  ctermui_component_t game = new_game_component(root->width, root->height);
  ctermui_layout_add_component(root, game);
  ctermui_screen_set_layout_root(screen, root);
  register_key_handlers(screen);
  ctermui_screen_loop_start(screen, periodic, 100);
  return 0;
}
