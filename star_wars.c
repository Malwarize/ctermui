#include "ctermui/ctermui_screen.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define ENEMY_NUMBER 20

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

void game_create(ctermui_screen_t s, ctermui_component c) {
    game.player.x = c->absolute_width / 2;
    game.player.y = c->absolute_height-1;
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        game.enemys[i].x = rand()%c->absolute_width;
        game.enemys[i].y = rand()%c->absolute_height/4;
    }
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        game.bullets[i].x = game.player.x;
        game.bullets[i].y = -1;
    }
}

void enemy_horizontal_move(ctermui_screen_t s, ctermui_component c) {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        //random step
        int step = rand()%2;
        if (step) {
            game.enemys[i].x = ((game.enemys[i].x - 1) + c->absolute_width) % c->absolute_width;
        }else{
            game.enemys[i].x = ((game.enemys[i].x + 1) + c->absolute_width) % c->absolute_width;
        }
    }
}

void bullet_create(int target) {
    game.bullets[target].x = game.player.x;
    game.bullets[target].y = game.player.y;
}

void bullet_vertical_move(ctermui_screen_t s, ctermui_component c, int target) {
    game.bullets[target].y = (game.bullets[target].y - 1);
}

void draw_bullet(ctermui_screen_t s, ctermui_component c, int target) {
    if (game.bullets[target].y < 0) {
        return;
    }
    ctermui_pencil_draw_char(s->buffer, game.bullets[target].x, game.bullets[target].y, ' ', CTERMUI_BRIGHT_YELLOW, CTERMUI_BRIGHT_YELLOW);
}

void draw_bullets(ctermui_screen_t s, ctermui_component c) {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.bullets[i].y < c->absolute_y) {
            continue;
        }
        draw_bullet(s, c, i);
    }
}

void draw_bullet_and_move(ctermui_screen_t s, ctermui_component c, int target) {
    draw_bullet(s, c, target);
    bullet_vertical_move(s, c, target);
}

void draw_bullets_and_move(ctermui_screen_t s, ctermui_component c) {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        draw_bullet_and_move(s, c, i);
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

void draw_enemys(ctermui_screen_t s, ctermui_component c) {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.enemys[i].y < c->absolute_y) {
            continue;
        }
        ctermui_pencil_draw_char(s->buffer, game.enemys[i].x, game.enemys[i].y, ' ', CTERMUI_BRIGHT_RED, CTERMUI_BRIGHT_RED);
    }
}

void draw_player(ctermui_screen_t s, ctermui_component c) {
    ctermui_pencil_draw_char(s->buffer, game.player.x, game.player.y, ' ', CTERMUI_BRIGHT_GREEN, CTERMUI_BRIGHT_GREEN);
}

void game_animate(ctermui_screen_t s, ctermui_component c) {
    draw_enemys(s, c);
    draw_player(s, c);
    draw_bullets_and_move(s, c);
    enemy_horizontal_move(s, c);
}

void move_player_left(void* data){
    ctermui_component c = (ctermui_component)data;
    game.player.x = ((game.player.x - 1 + c->absolute_width)%(c->absolute_width));
}

void move_player_right(void* data){
    ctermui_component c = (ctermui_component)data;
    game.player.x = ((game.player.x + 1 + c->absolute_width)%(c->absolute_width));
}

void move_enmeies_down(ctermui_screen_t s, ctermui_component c) {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.enemys[i].y < c->absolute_y) {
            continue;
        }
        game.enemys[i].y = (game.enemys[i].y + 1)%c->absolute_height;
    }
}
        
void check_if_win(ctermui_screen_t s, ctermui_component c) {
    int win = 1;
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.enemys[i].y < c->absolute_y) {
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

void check_if_bullet_hit_enemy(ctermui_screen_t s, ctermui_component c) {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.bullets[i].y < c->absolute_y) {
            continue;
        }
        for (int j = 0; j < ENEMY_NUMBER; j++) {
            if (game.enemys[j].y < c->absolute_y) {
                continue;
            }
            if (game.bullets[i].x == game.enemys[j].x && game.bullets[i].y == game.enemys[j].y) {
                game.bullets[i].y = -1;
                game.enemys[j].y = -1;
            }
        }
    }
}

void check_if_enemy_hit_ground(ctermui_screen_t s, ctermui_component c) {
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        if (game.enemys[i].y < c->absolute_y) {
            continue;
        }
        if (game.enemys[i].y == c->absolute_height-1) {
            ctermui_restore_cursor();
            fprintf(stderr, "You Lose !\n");
            exit(0);
        }
    }
}

void periodic(ctermui_screen_t* sp){
    ctermui_screen_t s = *sp;
    if(s->loop_count == 0){
        game_create(s, ctermui_widget_find_component(s->root, "game"));
    }
    if(s->loop_count%100 == 0){
            move_enmeies_down(s, ctermui_widget_find_component(s->root, "game"));
    }
    check_if_bullet_hit_enemy(s, ctermui_widget_find_component(s->root, "game"));
    check_if_enemy_hit_ground(s, ctermui_widget_find_component(s->root, "game"));
    check_if_win(s, ctermui_widget_find_component(s->root, "game"));
    ctermui_screen_refresh_widget(s, s->root);
}

void draw_game(ctermui_screen_t s, ctermui_component c){
    game_animate(s,c);
}

void calculate_absolute_position(ctermui_component c, int x, int y, int width, int height){
    c->absolute_x = x;
    c->absolute_y = y;
    c->absolute_width =width;
    c->absolute_height = height;
    game.player.x = (game.player.x * c->absolute_width) / width;
    game.player.y = c->absolute_height-1;
    for (int i = 0; i < ENEMY_NUMBER; i++) {
        game.enemys[i].x = (game.enemys[i].x * c->absolute_width) / width % c->absolute_width;
        game.enemys[i].y = (game.enemys[i].y * c->absolute_height) / height % c->absolute_height;
    }

}

int main() {
    s = ctermui_screen_new();
    ctermui_screen_keyboard_events_register(
        s->keyboard_events,
        'q', 
        (void*) exit,
        0
    );
    ctermui_screen_keyboard_events_register(
        s->keyboard_events, ' ', (void*) shoot, NULL
    );
    ctermui_widget root = ctermui_widget_new_root(
        LEAF,s->width, s->height
    );
    ctermui_screen_set_widget_root(
        s, root
    );
    ctermui_widget_add_component(root, ctermui_new_custom_component("game", draw_game, calculate_absolute_position));
    ctermui_widget_add_component(
            root,
            ctermui_new_soft_background("background", CTERMUI_BLACK, s->width, s->height)
    );
    /* ctermui_widget_add_component( */
    /*         root, */
    /*         ctermui_new_frame("frame", CTERMUI_WHITE, CTERMUI_WHITE) */
    /* ); */
    ctermui_screen_keyboard_events_register(
        s->keyboard_events,CTERMUI_KEY_RIGHT, move_player_right,
        ctermui_widget_find_component(s->root, "game")
    );
        
    ctermui_screen_keyboard_events_register(
        s->keyboard_events,
        CTERMUI_KEY_LEFT,
        move_player_left,
        ctermui_widget_find_component(s->root, "game")
    ); 
    
    ctermui_screen_loop_start(s, periodic, 10000);
    return 0;
}
