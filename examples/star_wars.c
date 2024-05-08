#include "ctermui_pencil.h"
#include "ctermui_screen.h"
#include <stdio.h>

#define ENEMY_NUMBER 100
#define BULLET_NUMBER 10

ctermui_screen_t screen;

typedef struct enemy {
  int x;
  int y;
} Enemy;

typedef struct player {
  int x;
  int y;
} Player;

typedef struct bullet {
  int x;
  int y;
} Bullet;

typedef struct game {
  Player player;
  Bullet bullets[BULLET_NUMBER];
  Enemy enemies[ENEMY_NUMBER];
} Game;

Game game;

void game_create(ctermui_screen_t s, ctermui_component_t c) {
  game.player.x = (int)c->width / 2;
  game.player.y = (int)c->height - 1;
  for (int i = 0; i < ENEMY_NUMBER; i++) {
    game.enemies[i].x = rand() % c->width;
    game.enemies[i].y = rand() % c->height / 4;
  }
  for (int i = 0; i < BULLET_NUMBER; i++) {
    game.bullets[i].x = game.player.x;
    game.bullets[i].y = -1;
  }
}

void enemy_horizontal_move(ctermui_screen_t s, ctermui_component_t c) {
  for (int i = 0; i < ENEMY_NUMBER; i++) {
    // random step
    int step = rand() % 2;
    if (step) {
      game.enemies[i].x =
          ((game.enemies[i].x - 1) + (int)c->width) % (int)c->width;
    } else {
      game.enemies[i].x =
          ((game.enemies[i].x + 1) + (int)c->width) % (int)c->width;
    }
  }
}

void bullet_create(int target) {
  game.bullets[target].x = game.player.x;
  game.bullets[target].y = game.player.y;
}

void bullet_vertical_move(ctermui_screen_t s, ctermui_component_t c,
                          int target) {
  game.bullets[target].y = (game.bullets[target].y - 1);
}

void draw_bullet(ctermui_screen_t s, ctermui_component_t c, int target) {
  if (game.bullets[target].y < 0) {
    return;
  }
  ctermui_pencil_draw_char(s->buffer, game.bullets[target].x,
                           game.bullets[target].y, ' ', CTERMUI_BRIGHT_YELLOW,
                           CTERMUI_BRIGHT_YELLOW, DEFAULT);
}

void draw_bullets(ctermui_screen_t s, ctermui_component_t c) {
  for (int i = 0; i < BULLET_NUMBER; i++) {
    if (game.bullets[i].y < c->y) {
      continue;
    }
    draw_bullet(s, c, i);
  }
}

void draw_bullet_and_move(ctermui_screen_t s, ctermui_component_t c,
                          int target) {
  draw_bullet(s, c, target);
  bullet_vertical_move(s, c, target);
}

void draw_bullets_and_move(ctermui_screen_t s, ctermui_component_t c) {
  for (int i = 0; i < BULLET_NUMBER; i++) {
    draw_bullet_and_move(s, c, i);
  }
}

void shoot() {
  for (int i = 0; i < BULLET_NUMBER; i++) {
    if (game.bullets[i].y < 0) {
      bullet_create(i);
      return;
    }
  }
}

void draw_enemies(ctermui_screen_t s, ctermui_component_t c) {
  for (int i = 0; i < ENEMY_NUMBER; i++) {
    if (game.enemies[i].y < 0) {
      continue;
    }
    ctermui_pencil_draw_char(s->buffer, game.enemies[i].x, game.enemies[i].y,
                             ' ', CTERMUI_BRIGHT_RED, CTERMUI_BRIGHT_RED,
                             DEFAULT);
  }
}

void draw_player(ctermui_screen_t s, ctermui_component_t c) {
  ctermui_pencil_draw_char(s->buffer, game.player.x, game.player.y, ' ',
                           CTERMUI_BRIGHT_GREEN, CTERMUI_BRIGHT_GREEN, DEFAULT);
}

void game_animate(ctermui_screen_t s, ctermui_component_t c) {
  draw_enemies(s, c);
  draw_player(s, c);
  draw_bullets_and_move(s, c);
  enemy_horizontal_move(s, c);
}

void move_player_left(void *data) {
  ctermui_component_t c = (ctermui_component_t)data;
  game.player.x = ((game.player.x - 1 + (int)c->width) % (int)(c->width));
}

void move_player_right(void *data) {
  ctermui_component_t c = (ctermui_component_t)data;
  game.player.x = ((game.player.x + 1 + (int)c->width) % (int)(c->width));
}

void move_enemies_down(ctermui_screen_t s, ctermui_component_t c) {
  for (int i = 0; i < ENEMY_NUMBER; i++) {
    if (game.enemies[i].y < 0) {
      continue;
    }
    game.enemies[i].y = (game.enemies[i].y + 1);
  }
}

void check_if_win(ctermui_screen_t s, ctermui_component_t c) {
  int win = 1;
  for (int i = 0; i < ENEMY_NUMBER; i++) {
    if (game.enemies[i].y < 0) {
      continue;
    }
    win = 0;
  }
  if (win) {
    ctermui_restore_cursor();
    fprintf(stderr, "You Win !\n");
    exit(0);
  }
}

void check_if_bullet_hit_enemy(ctermui_screen_t s, ctermui_component_t c) {
  for (int i = 0; i < BULLET_NUMBER; i++) {
    if (game.bullets[i].y < c->y) {
      continue;
    }
    for (int j = 0; j < ENEMY_NUMBER; j++) {
      if (game.enemies[j].y < 0) {
        continue;
      }
      if (game.bullets[i].x == game.enemies[j].x &&
          game.bullets[i].y == game.enemies[j].y) {
        game.bullets[i].y = -1;
        game.enemies[j].y = -1;
      }
    }
  }
}

void check_if_enemy_hit_ground(ctermui_screen_t s, ctermui_component_t c) {
  for (int i = 0; i < ENEMY_NUMBER; i++) {
    if (game.enemies[i].y < 0) {
      continue;
    }
    if (game.enemies[i].y == c->height - 1) {
      ctermui_restore_cursor();
      fprintf(stderr, "You Lose !\n");
      exit(0);
    }
  }
}

void periodic(ctermui_screen_t *sp) {
  ctermui_screen_t s = *sp;
  if (s->loop_count == 0) {
    game_create(s, ctermui_layout_find_component(s->root, "game"));
  }
  if (s->loop_count % 100 == 0) {
    move_enemies_down(s, ctermui_layout_find_component(s->root, "game"));
  }
  check_if_bullet_hit_enemy(s, ctermui_layout_find_component(s->root, "game"));
  check_if_enemy_hit_ground(s, ctermui_layout_find_component(s->root, "game"));
  check_if_win(s, ctermui_layout_find_component(s->root, "game"));
  ctermui_screen_refresh_layout(s, s->root);
}

void draw_game(ctermui_screen_t s, ctermui_component_t c) {
  game_animate(s, c);
}

void calculate_absolute_position(ctermui_component_t c, size_t x, size_t y,
                                 size_t width, size_t height) {
  c->x = x;
  c->y = y;
  c->width = width;
  c->height = height;
  game.player.x = ((game.player.x + (int)c->width) % (int)(c->width));
  game.player.y = (int)c->height - 1;
  for (size_t i = 0; i < ENEMY_NUMBER; i++) {
    if (game.enemies[i].y < 0) {
      continue;
    }
    game.enemies[i].x = ((game.enemies[i].x + (int)c->width) % (int)(c->width));
    game.enemies[i].y =
        ((game.enemies[i].y + (int)c->height) % (int)(c->height));
  }
}

int main() {
  screen = ctermui_screen_init();
  ctermui_screen_keyboard_events_register(screen->keyboard_events, 'q',
                                          (void *)exit, 0);
  ctermui_screen_keyboard_events_register(screen->keyboard_events, ' ',
                                          (void *)shoot, NULL);
  ctermui_layout_t root =
      ctermui_layout_new_root(LEAF, screen->width, screen->height);
  ctermui_screen_set_layout_root(screen, root);
  ctermui_layout_add_component(
      root, ctermui_new_custom_component("game", &game, draw_game,
                                         calculate_absolute_position));
  ctermui_layout_add_component(
      root, ctermui_new_soft_background("background", CTERMUI_BLACK));
  ctermui_screen_keyboard_events_register(
      screen->keyboard_events, CTERMUI_KEY_RIGHT, move_player_right,
      ctermui_layout_find_component(screen->root, "game"));

  ctermui_screen_keyboard_events_register(
      screen->keyboard_events, CTERMUI_KEY_LEFT, move_player_left,
      ctermui_layout_find_component(screen->root, "game"));

  ctermui_screen_loop_start(screen, periodic, 10000);
  return 0;
}
