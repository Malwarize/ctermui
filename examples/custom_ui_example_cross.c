//#include "ctermui/ctermui_screen.h"
//#include "ctermui/ctermui_pencil.h"
//#define ENEMY_NUMBER 10
//#define BULLET_NUMBER 10
//
//enum BulletState {
//  BULLET_FIRED,
//  BULLET_NOT_FIRED
//};
//
//enum EnemyState {
//  ENEMY_ALIVE,
//  ENEMY_DEAD
//};
//
//typedef struct enemy {
//  int x;
//  int y;
//  int state;
//} Enemy;
//
//typedef struct player {
//  int x;
//  int y;
//} Player;
//
//typedef struct bullet {
//  int x;
//  int y;
//  int state;
//} Bullet;
//
//typedef struct game {
//  Player player;
//  Bullet bullets[ENEMY_NUMBER];
//  Enemy enemies[BULLET_NUMBER];
//} Game;
//
//
//void shoot(void* arg){
//  ctermui_component c = (ctermui_component)arg;
//  Game* game = (Game*)c->core_component;
//  for(int i = 0; i < BULLET_NUMBER; i++){
//    if(game->bullets[i].state == BULLET_NOT_FIRED){
//      game->bullets[i].state = BULLET_FIRED;
//      game->bullets[i].x = game->player.x;
//      game->bullets[i].y = game->player.y;
//      break;
//    }
//  }
//}
//
//void load_one_bullet(void* arg){
//  ctermui_component c = (ctermui_component)arg;
//  Game* game = (Game*)c->core_component;
//  for(int i = 0; i < BULLET_NUMBER; i++){
//    if(game->bullets[i].state == BULLET_NOT_FIRED){
//      game->bullets[i].state = BULLET_FIRED;
//      game->bullets[i].x = game->player.x;
//      game->bullets[i].y = game->player.y;
//      break;
//    }
//  }
//}
//void check_bullet_collision(Game* game){
//  for(int i = 0; i < BULLET_NUMBER; i++){
//    if(game->bullets[i].state == BULLET_FIRED){
//      for(int j = 0; j < ENEMY_NUMBER; j++){
//        if(game->enemies[j].state == ENEMY_ALIVE){
//          if(game->bullets[i].x == game->enemies[j].x &&
//             game->bullets[i].y == game->enemies[j].y){
//            game->bullets[i].state = BULLET_NOT_FIRED;
//            game->enemies[j].state = ENEMY_DEAD;
//          }
//        }
//      }
//    }
//  }
//}
//
//void enemy_horizontal_move(ctermui_component c){
//  Game* game = (Game*)c->core_component;
//  for(int i = 0; i < ENEMY_NUMBER; i++){
//    int step = rand() % 2;
//    if(step){
//      game->enemies[i].x = ((game->enemies[i].x - 1) + (int)c->absolute_width) % (int)c->absolute_width;
//    }else{
//      game->enemies[i].x = ((game->enemies[i].x + 1) + (int)c->absolute_width) % (int)c->absolute_width;
//    }
//  }
//}
//
//void enemy_vertical_move(ctermui_component c){
//  Game* game = (Game*)c->core_component;
//  for(int i = 0; i < ENEMY_NUMBER; i++){
//    int step = rand() % 2;
//    if(step){
//      game->enemies[i].y = ((game->enemies[i].y - 1) + (int)c->absolute_height) % (int)c->absolute_height;
//    }else{
//      game->enemies[i].y = ((game->enemies[i].y + 1) + (int)c->absolute_height) % (int)c->absolute_height;
//    }
//  }
//}
//
//void bullet_vertical_move(ctermui_component c){
//  Game* game = (Game*)c->core_component;
//  for(int i = 0; i < BULLET_NUMBER; i++){
//    if(game->bullets[i].state == BULLET_FIRED){
//      game->bullets[i].y = ((game->bullets[i].y - 1) + (int)c->absolute_height) % (int)c->absolute_height;
//    }
//  }
//}
//
//
//void draw_game(ctermui_screen_t s, ctermui_component c){
//  Game* game = (Game*)c->core_component;
//  for(int i = 0; i < ENEMY_NUMBER; i++){
//    if(game->enemies[i].state == ENEMY_ALIVE){
//      //\U0001F47E"
//      ctermui_pencil_draw_text(s->buffer, game->player.x, game->player.y, "E", CTERMUI_RED, CTERMUI_BLACK);
//    }
//  }
//  for(int i = 0; i < BULLET_NUMBER; i++){
//    if(game->bullets[i].state == BULLET_FIRED){
//      //xF0x9Fx94xA5
//      ctermui_pencil_draw_text(s->buffer, game->bullets[i].x, game->bullets[i].y, "B", CTERMUI_RED, CTERMUI_BLACK);
//    }
//  }
//  //xF0x9Fx9BxB8
//  ctermui_pencil_draw_text(s->buffer, game->player.x, game->player.y, "P", CTERMUI_RED, CTERMUI_BLACK);
//}
//
//
//void calculate_abs_position_game(ctermui_component c,
//                                 size_t parent_x,
//                                 size_t parent_y,
//                                 size_t parent_width,
//                                 size_t parent_height){
//  c->absolute_x = parent_x;
//  c->absolute_y = parent_y;
//  c->absolute_width = parent_width;
//  c->absolute_height = parent_height;
//  Game* game = (Game*)c->core_component;
//  game->player.x = (game->player.x * (int) c->absolute_width) / (int) parent_width;
//  game->player.y = (int) c->absolute_height - 1;
//  for (size_t i = 0; i < ENEMY_NUMBER; i++) {
//    game->enemies[i].x =
//      (game->enemies[i].x * c->absolute_width) / parent_width %
//      c->absolute_width;
//    if(game->enemies[i].y >= 0)
//      game->enemies[i].y =
//        (game->enemies[i].y * c->absolute_height ) / parent_height % (c->absolute_height);
//  }
//}
//void game_create(ctermui_component c){
//  Game* game = (Game*)c->core_component;
//  game->player.x =(int) c->absolute_width / 2;
//  game->player.y = (int)  c->absolute_height - 1;
//  for (int i = 0; i < ENEMY_NUMBER; i++) {
//    game->enemies[i].x = rand() % (int) c->absolute_width;
//    game->enemies[i].y = rand() % (int) c->absolute_height / 4;
//    game->enemies[i].state = ENEMY_ALIVE;
//  }
//  for (int i = 0; i < BULLET_NUMBER; i++) {
//    game->bullets[i].x = game->player.x;
//    game->bullets[i].y = game->player.y;
//    game->bullets[i].state = BULLET_NOT_FIRED;
//  }
//}
//
//void periodic(ctermui_screen_t *sp){
//  ctermui_screen_t s = *sp;
//  ctermui_component game_component = ctermui_widget_find_component(s->root, "game");
//  enemy_horizontal_move(game_component);
//  bullet_vertical_move(game_component);
//  if(s->loop_count % 10000 == 0){
//    enemy_vertical_move(game_component);
//    load_one_bullet(game_component);
//  }
//  check_bullet_collision((Game*)game_component->core_component);
//}
//
//int main(){
//  Game game;
//  ctermui_screen_t s = ctermui_screen_init();
//  ctermui_widget_t root = ctermui_widget_new_root(LEAF, s->width, s->height);
//  ctermui_component game_component = ctermui_new_custom_component(
//    "game",draw_game,calculate_abs_position_game, &game);
//  game_create(game_component);
//  ctermui_widget_add_component(root, game_component);
//  ctermui_screen_set_widget_root(s, root);
//  ctermui_screen_loop_start(s, periodic, 1000);
//}
#include "ctermui/ctermui_screen.h"
#include "ctermui/ctermui_pencil.h"
// to make a custom component, you need to define a struct
typedef struct CustomComponentCross {
  int color;
}CustomComponentCross;


// make a function to draw the component

void draw_custom_component(ctermui_screen_t s, ctermui_component c)
{
  CustomComponentCross* cross = (CustomComponentCross*)c->core_component; // cast this fucking bastard
  size_t vertical_starting_point = c->absolute_x + c->absolute_width / 2;
  ctermui_pencil_draw_line(
    s->buffer,
    CTERMUI_VERTICAL,
    vertical_starting_point,
    c->absolute_y,
    c->absolute_height,
    CTERMUI_VERTICAL_LINE,
    cross->color,
    cross->color
  );

  size_t horizontal_starting_point = c->absolute_y + c->absolute_height / 2;
  ctermui_pencil_draw_line(
    s->buffer,
    CTERMUI_HORIZONTAL,
    c->absolute_x,
    horizontal_starting_point,
    c->absolute_width,
    CTERMUI_VERTICAL_LINE,
    cross->color,
    cross->color
  );
}

// make a function that calculates the absolute position of the component
void calculate_abs_position_custom_component(ctermui_component c,
                                             size_t parent_x,
                                             size_t parent_y,
                                             size_t parent_width,
                                             size_t parent_height)
{
  c->absolute_x = parent_x;
  c->absolute_y = parent_y;
  c->absolute_width = parent_width;
  c->absolute_height = parent_height;
}
//
int main()
{
  ctermui_screen_t screen = ctermui_screen_init();
  ctermui_widget_t root = ctermui_widget_new_root(
    CTERMUI_HORIZONTAL, screen->width, screen->height);


  CustomComponentCross cross = {
    .color = CTERMUI_RED
  };
  ctermui_component custom_component = ctermui_new_custom_component(
    "custom_component", draw_custom_component, calculate_abs_position_custom_component, (void*)&cross);
  ctermui_widget_add_component(root, custom_component);
  ctermui_screen_set_widget_root(screen, root);
  ctermui_screen_loop_start(screen, NULL, 0);
  return 0;
}