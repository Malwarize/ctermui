#include "../include/ctermui_component.h"
#include "../include/ctermui_events.h"
#include "../include/ctermui_layout.h"
#include "../include/ctermui_screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
enum Direction { UP, DOWN, LEFT, RIGHT };

typedef struct Node {
  int x;
  int y;
  struct Node *next;
  struct Node *prev;
} Node;

typedef struct Deque {
  Node *head;
  Node *tail;
} Deque;

Deque *createDeque() {
  Deque *deque = (Deque *)malloc(sizeof(Deque));
  if (deque == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  deque->head = NULL;
  deque->tail = NULL;
  return deque;
}

int isEmpty(Deque *deque) { return (deque->head == NULL); }

void addToFront(Deque *deque, int x, int y) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (newNode == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  newNode->x = x;
  newNode->y = y;
  newNode->next = deque->head;
  newNode->prev = NULL;

  if (isEmpty(deque)) {
    deque->tail = newNode;
  } else {
    deque->head->prev = newNode;
  }
  deque->head = newNode;
}

void addToBack(Deque *deque, int x, int y) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (newNode == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  newNode->x = x;
  newNode->y = y;
  newNode->next = NULL;
  newNode->prev = deque->tail;

  if (isEmpty(deque)) {
    deque->head = newNode;
  } else {
    deque->tail->next = newNode;
  }
  deque->tail = newNode;
}

int *removeFromFront(Deque *deque) {
  if (isEmpty(deque)) {
    printf("Deque is empty\n");
    exit(1);
  }
  Node *temp = deque->head;
  int *data = (int *)malloc(2 * sizeof(int));
  if (data == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  data[0] = temp->x;
  data[1] = temp->y;
  deque->head = temp->next;
  if (deque->head == NULL) {
    deque->tail = NULL;
  } else {
    deque->head->prev = NULL;
  }
  free(temp);
  return data;
}

int *removeFromBack(Deque *deque) {
  if (isEmpty(deque)) {
    printf("Deque is empty\n");
    exit(1);
  }
  Node *temp = deque->tail;
  int *data = (int *)malloc(2 * sizeof(int));
  if (data == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  data[0] = temp->x;
  data[1] = temp->y;
  deque->tail = temp->prev;
  if (deque->tail == NULL) {
    deque->head = NULL;
  } else {
    deque->tail->next = NULL;
  }
  free(temp);
  return data;
}
void freeDeque(Deque *deque) {
  Node *current = deque->head;
  Node *temp;
  while (current != NULL) {
    temp = current;
    current = current->next;
    free(temp);
  }
  free(deque);
}
typedef Deque *WormBody;

typedef struct Worm {
  WormBody body;
  size_t food_x, food_y;
  enum Direction previous_direction;
  enum Direction direction;

  int score;

} *Worm;

int check_game_over(ctermui_screen_t screen, ctermui_component_t c) {
  Worm worm = c->core_component;
  Node *head = worm->body->head;
  Node *n = head->next;
  while (n != NULL) {
    if (head->x == n->x && head->y == n->y) {
      return 1;
    }
    n = n->next;
  }
  if (head->x < c->parent->x + 1 || head->x >= c->parent->width - 1 ||
      head->y < c->parent->y + 1 || head->y >= c->parent->height - 1) {
    return 1;
  }

  return 0;
}
void draw_worm(ctermui_screen_t screen, ctermui_component_t c) {
  Worm worm = c->core_component;
  Node *n = worm->body->head;
  if (check_game_over(screen, c)) {
    ctermui_screen_loop_shutdown(screen);
    printf("\nGame Over\n");
    exit(0);
  }
  while (n != NULL) {
    ctermui_pencil_draw_char(screen->buffer, n->x, n->y, ' ',
                             CTERMUI_BRIGHT_RED, CTERMUI_BRIGHT_RED, 1);

    n = n->next;
  }
  // draw food
  ctermui_pencil_draw_char(screen->buffer, worm->food_x, worm->food_y, ' ',
                           CTERMUI_BRIGHT_GREEN, CTERMUI_BRIGHT_GREEN, 1);
}

void calculate_absolute_position(struct ctermui_component *c, size_t parent_x,
                                 size_t parent_y, size_t parent_width,
                                 size_t parent_height) {
  Worm w = c->core_component;
  Node *head = w->body->head;
  c->x = parent_x + head->x % parent_width;
  c->y = parent_y + head->y % parent_height;
  c->width = parent_width;
  c->height = parent_height;
}

ctermui_component_t create_worm(size_t x, size_t y) {
  Worm worm = malloc(sizeof(struct Worm));
  worm->direction = RIGHT;
  worm->previous_direction = RIGHT;
  worm->body = createDeque();
  worm->score = 0;
  // add initial worm body
  for (int i = 1; i < 4; i++) {
    addToBack(worm->body, x - i, y);
  }
  return ctermui_new_custom_component("worm", worm, draw_worm,
                                      calculate_absolute_position);
}
// generate food
void generate_food(ctermui_component_t c) {
  Worm w = c->core_component;
  // border 1 px
  // random x and y
  w->food_x = (rand() % (c->parent->width - 2)) + c->parent->x + 1;
  w->food_y = (rand() % (c->parent->height - 2)) + c->parent->y + 1;
  Node *n = w->body->head;
  while (n != NULL) {
    if (n->x == w->food_x && n->y == w->food_y) {
      generate_food(c);
      return;
    }
    n = n->next;
  }
}
/* worm control */
void move_worm(ctermui_screen_t screen, ctermui_component_t c) {
  Worm w = c->core_component;
  switch (w->direction) {
  case LEFT:
    if (w->previous_direction == RIGHT) {
      w->direction = RIGHT;
    } else {
      w->previous_direction = LEFT;
    }
    break;
  case RIGHT:
    if (w->previous_direction == LEFT) {
      w->direction = LEFT;
    } else {
      w->previous_direction = RIGHT;
    }
    break;
  case UP:
    if (w->previous_direction == DOWN) {
      w->direction = DOWN;
    } else {
      w->previous_direction = UP;
    }
    break;
  case DOWN:
    if (w->previous_direction == UP) {
      w->direction = UP;
    } else {
      w->previous_direction = DOWN;
    }
    break;
  }

  Node *head = w->body->head;
  int x = head->x;
  int y = head->y;

  switch (w->direction) {
  case LEFT:
    x--;
    break;
  case RIGHT:
    x++;
    break;
  case UP:
    y--;
    break;
  case DOWN:
    y++;
    break;
  }

  addToFront(w->body, x, y);
  if (w->food_x == x && w->food_y == y) {

    generate_food(c);
    ctermui_layout_t score = ctermui_layout_find(screen->root, "score_layout");
    ctermui_layout_t game = ctermui_layout_find(screen->root, "game_layout");
    Worm w = game->components[0]->core_component;
    ctermui_component_t score_c = score->components[0];
    Text *t = score_c->core_component;
    w->score++;
    char score_str[20];
    sprintf(score_str, "Score: %d", w->score);
    strcpy(t->text, score_str);
  } else {
    int *tail = removeFromBack(w->body);
    free(tail);
  }
}

// game over
void periodic(ctermui_screen_t *sp) {

  ctermui_screen_t screen = *sp; // dereference

  if (screen->loop_count % 70 != 0) {
    return;
  }
  ctermui_layout_t game = ctermui_layout_find(screen->root, "game_layout");

  if (screen->loop_count == 0) {
    ctermui_component_t worm = game->components[0];
    generate_food(worm);
  }
  ctermui_layout_t root = screen->root;
  ctermui_component_t worm = game[0].components[0];
  Worm w = worm->core_component;
  // slow down the worm
  if (screen->loop_count % 140 == 0 &&
      (w->direction == UP || w->direction == DOWN)) {
    return;
  }
  move_worm(screen, worm);
  ctermui_screen_refresh_layout(screen, root);
}

void left_key_handler(void *direction) {
  enum Direction *d = direction;
  *d = LEFT;
}

void right_key_handler(void *direction) {
  enum Direction *d = direction;
  *d = RIGHT;
}

void up_key_handler(void *direction) {
  enum Direction *d = direction;
  *d = UP;
}

void down_key_handler(void *direction) {
  enum Direction *d = direction;
  *d = DOWN;
}
void register_key_handlers(ctermui_screen_t screen) {

  ctermui_layout_t game = ctermui_layout_find(screen->root, "game_layout");
  Worm w = game->components[0]->core_component;

  ctermui_screen_keyboard_events_register(screen->keyboard_events, 'h',
                                          left_key_handler, &(w->direction));

  ctermui_screen_keyboard_events_register(screen->keyboard_events,
                                          CTERMUI_KEY_LEFT, left_key_handler,
                                          &(w->direction));

  ctermui_screen_keyboard_events_register(screen->keyboard_events,
                                          CTERMUI_KEY_RIGHT, right_key_handler,
                                          &(w->direction));

  ctermui_screen_keyboard_events_register(screen->keyboard_events, 'l',
                                          right_key_handler, &(w->direction));

  ctermui_screen_keyboard_events_register(
      screen->keyboard_events, CTERMUI_KEY_UP, up_key_handler, &(w->direction));

  ctermui_screen_keyboard_events_register(screen->keyboard_events, 'k',
                                          up_key_handler, &(w->direction));

  ctermui_screen_keyboard_events_register(screen->keyboard_events,
                                          CTERMUI_KEY_DOWN, down_key_handler,
                                          &(w->direction));

  ctermui_screen_keyboard_events_register(screen->keyboard_events, 'j',
                                          down_key_handler, &(w->direction));
}

int main() {
  // random seed
  srand(time(NULL));
  ctermui_screen_t screen = ctermui_screen_init();
  ctermui_layout_t root = ctermui_layout_new_root(
      CTERMUI_HORIZONTAL, screen->width, screen->height);

  ctermui_layout_t game_l =
      ctermui_layout_new("game_layout", CTERMUI_VERTICAL, 100 / 2);
  ctermui_component_t worm = create_worm(5, 5);
  ctermui_layout_add_component(game_l, worm);

  ctermui_component_t frame =
      ctermui_new_frame("frame", CTERMUI_BRIGHT_BLUE, CTERMUI_BRIGHT_BLUE);
  ctermui_layout_add_component(game_l, frame);

  ctermui_layout_t score_l =
      ctermui_layout_new("score_layout", CTERMUI_VERTICAL, 100 / 2);
  ctermui_component_t score =
      ctermui_new_text("score", "Score: 0", CTERMUI_RED, CTERMUI_BRIGHT_WHITE,
                       CTERMUI_ALIGN_CENTER);
  ctermui_layout_add_component(score_l, score);

  ctermui_layout_add_child(root, game_l);
  ctermui_layout_add_child(root, score_l);
  ctermui_screen_set_layout_root(screen, root);
  register_key_handlers(screen);
  ctermui_screen_loop_start(screen, periodic, 1000);
}
