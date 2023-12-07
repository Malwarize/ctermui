#include "ctermui/ctermui_screen.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define ENEMY_NUMBER 10 

ctermui_screen_t s;

typedef struct enemy{
    int x;
    int y;
} Enemy;

typedef struct player{
    int x;
    int y;
} Player;

typedef struct bullet{
    int x;
    int y;
} Bullet;

typedef struct game{
    Player player;
    Bullet bullets[ENEMY_NUMBER];
    Enemy enemys[ENEMY_NUMBER];
} Game;

Game game;

void game_create() {
    game.player.x = s->width / 2;
    game.player.y = s->height-1;

    for (int i = 0; i < ENEMY_NUMBER; i++) {
        game.enemys[i].x = rand()%s->width;
        game.enemys[i].y = rand()%s->height/4;
    }
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        game.bullets[i].x = game.player.x;
        game.bullets[i].y = -1;
    }
}

void enemy_horizontal_move() {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        //random step
        int step = rand()%3;
        if (step == 0) {
            game.enemys[i].x = (game.enemys[i].x - 1)%(s->width);
        }
        if (step == 1) {
            game.enemys[i].x = (game.enemys[i].x + 1)%(s->width);
        }
    }
}

void bullet_create(int target) {
    game.bullets[target].x = game.player.x;
    game.bullets[target].y = game.player.y;
}

void bullet_vertical_move(int target) {
    game.bullets[target].y = (game.bullets[target].y - 1);
}

void draw_bullet(int target) {
    if (game.bullets[target].y < 0) {
        return;
    }
    ctermui_pencil_draw_char(s->buffer, game.bullets[target].x, game.bullets[target].y, ' ', CTERMUI_BRIGHT_YELLOW, CTERMUI_BRIGHT_YELLOW);
}

void draw_bullets() {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.bullets[i].y < 0) {
            continue;
        }
        draw_bullet(i);
    }
}

void draw_bullet_and_move(int target) {
    draw_bullet(target);
    bullet_vertical_move(target);
}

void draw_bullets_and_move() {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        draw_bullet_and_move(i);
    }
}
void shoot() {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.bullets[i].y < 0) {
            bullet_create(i);
            return;
        }
    }
}
void draw_enemys() {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.enemys[i].y < 0) {
            continue;
        }
        ctermui_pencil_draw_char(s->buffer, game.enemys[i].x, game.enemys[i].y, ' ', CTERMUI_BRIGHT_RED, CTERMUI_BRIGHT_RED);
    }
}

void draw_player() {
    ctermui_pencil_draw_char(s->buffer, game.player.x, game.player.y, ' ', CTERMUI_BRIGHT_GREEN, CTERMUI_BRIGHT_GREEN);
}

void game_animate() {
    draw_enemys();
    draw_player();
    draw_bullets_and_move();
    enemy_horizontal_move();
}
void move_player_left() {
    game.player.x = (game.player.x - 1)%(s->width);
}
void move_player_right() {
    game.player.x = (game.player.x + 1)%(s->width);
}
void move_enmeies_down() {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.enemys[i].y < 0) {
            continue;
        }
        game.enemys[i].y = (game.enemys[i].y + 1)%(s->height);
    }
}
void check_if_win(){
    int win = 1;
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.enemys[i].y < 0) {
            continue;
        }
        win = 0;
    }
    if(win){
        ctermui_restore_cursor();
        fprintf(stderr, "You Win !\n");
        exit(0);
    }
}
void check_if_bullet_hit_enemy() {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.bullets[i].y < 0) {
            continue;
        }
        for (int j = 0; j < ENEMY_NUMBER; j++) {
            if (game.enemys[j].y < 0) {
                continue;
            }
            if (game.bullets[i].x == game.enemys[j].x && game.bullets[i].y == game.enemys[j].y) {
                game.bullets[i].y = -1;
                game.enemys[j].y = -1;
            }
        }
    }
}

void check_if_enemy_hit_ground() {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.enemys[i].y < 0) {
            continue;
        }
        if (game.enemys[i].y == s->height-1) {
            ctermui_restore_cursor();
            fprintf(stderr, "You Lose !\n");
            exit(0);
        }
    }
}
void periodic(ctermui_screen_t s){
    if(s->loop_count%100 == 0){
            move_enmeies_down();
    }
    check_if_bullet_hit_enemy();
    check_if_enemy_hit_ground();
    check_if_win();
    ctermui_screen_refresh_widget(s, s->root, s->root->absolute_x, s->root->absolute_y, s->root->absolute_width, s->root->absolute_height);
}

void draw_game(ctermui_screen_t s, ctermui_component c){
    game_animate();
}
int main() {
    s = ctermui_screen_new();
        
    ctermui_screen_keyboard_events_register(s->keyboard_events, 'q', (void*) exit, s);
    ctermui_screen_keyboard_events_register(s->keyboard_events, ' ', (void*) shoot, NULL);
    ctermui_screen_keyboard_events_register(s->keyboard_events,CTERMUI_KEY_RIGHT, move_player_right, NULL);
    ctermui_screen_keyboard_events_register(s->keyboard_events, CTERMUI_KEY_LEFT, move_player_left, NULL); 
    
    ctermui_widget root = ctermui_widget_new_root(LEAF,s->width, s->height);
    ctermui_screen_set_widget_root(s, root);
    ctermui_component ctermui_new_custom_component(char* id, void (*draw)(ctermui_screen_t s, ctermui_component c));
    ctermui_widget_add_component(root, ctermui_new_custom_component("game", draw_game));
    game_create();

    ctermui_screen_loop_start(s, periodic, 10000);
    return 0;
}

