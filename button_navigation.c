#include "ctermui/ctermui_screen.h"
#include <stdio.h>
#include <unistd.h>

ctermui_screen_t s;
Button* btnList[4];
int bindex = 0;

void on_click(void* arg){
    Button* btn =   btnList[bindex];
    strcpy(btn->text, "clicked");
    ctermui_screen_refresh_widgets(s);
}

void select_button(Button* btn){
    btn->text_color = CTERMUI_BRIGHT_YELLOW;
    btn->bg_color = CTERMUI_RED;
}
void unselect_button(Button* btn){
    btn->text_color = CTERMUI_BLACK;
    btn->bg_color = CTERMUI_BRIGHT_YELLOW;
}

void on_select_arrow_up(void* arg){
    unselect_button(btnList[bindex]);
    bindex = (bindex-1)%4;
    if(bindex < 0){
        bindex = 3;
    }
    select_button(btnList[bindex]);
    ctermui_screen_refresh_widgets(s);
}
void on_select_arrow_down(void* arg){
    unselect_button(btnList[bindex]);
    bindex = (bindex+1)%4;
    select_button(btnList[bindex]);
    ctermui_screen_refresh_widgets(s);
}

void periodic(ctermui_screen_t s){
    ctermui_widget w1 = ctermui_widget_find(s->root, "child1");
    ctermui_component c1 = ctermui_widget_find_component(w1, "btn1");
    Button* btn = (Button*)c1->core_component;
    sprintf(btn->text,"%d", s->loop_count);
    btn->bg_color = CTERMUI_BRIGHT_CYAN;

    if(s->loop_count == 2000){
        ctermui_screen_loop_pause(s);
        usleep(1000000*5);
        ctermui_screen_loop_resume(s);
    }
    ctermui_screen_refresh_widget(s, w1, w1->absolute_x, w1->absolute_y, w1->absolute_width, w1->absolute_height);
}

void ButtonSelectionExample(){
    s = ctermui_screen_new();
    ctermui_widget root = ctermui_widget_new_root(CTERMUI_VERTICAL,s->width, s->height);
    for (size_t i = 0; i < 4; i++)
    {
        char id[100];
        sprintf(id, "child%zu", i);
        ctermui_widget child1 = ctermui_widget_new(id, CTERMUI_HORIZONTAL, 20);
        ctermui_component btn1 = ctermui_new_button(
            "btn1",
            "select me",
            CTERMUI_ALIGN_CENTER,
            CTERMUI_BLACK,
            CTERMUI_BRIGHT_YELLOW
        );
        ctermui_widget_add_component(child1, btn1);
        ctermui_widget_add_child(root, child1);
        btnList[i] = (Button*)btn1->core_component;
    }
    
    ctermui_screen_keyboard_events_register(s->keyboard_events, 65, on_select_arrow_up, NULL);
    ctermui_screen_keyboard_events_register(s->keyboard_events, 66, on_select_arrow_down, NULL);

    ctermui_screen_keyboard_events_register(s->keyboard_events, 10, on_click, NULL);
    ctermui_screen_set_widget_root(s, root);
    ctermui_screen_loop_start(s, periodic, 100);
}
int main() {
    ButtonSelectionExample();
    return 0;
}
