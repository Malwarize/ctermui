#include "ctermui/ctermui_screen.h"
#include <stdio.h>

ctermui_screen_t s;
Button* btnList[4];
int bindex = 0;

void on_click(void* arg){
    Button* btn =   btnList[bindex];
    btn->text = "clicked";
    ctermui_screen_refresh(s);
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
    ctermui_screen_refresh(s);
}
void on_select_arrow_down(void* arg){
    unselect_button(btnList[bindex]);
    bindex = (bindex+1)%4;
    select_button(btnList[bindex]);
    ctermui_screen_refresh(s);
}

void ButtonSelectionExample(){
    s = ctermui_screen_new();
    ctermui_widget root = ctermui_widget_new_root(CTERMUI_VERTICAL,s->width, s->height);
    for (size_t i = 0; i < 4; i++)
    {
        ctermui_widget child1 = ctermui_widget_new(LEAF, 20);
        ctermui_component btn1 = ctermui_new_button(
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
    ctermui_start(s);
}
int main() {
    ButtonSelectionExample();
    return 0;
}
