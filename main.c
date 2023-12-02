#include "ctermui/ctermui_screen.h"
#include <stdio.h>

void its_test(){
    ctermui_screen_t s = ctermui_screen_new();

    ctermui_widget root = ctermui_widget_new_root(CTERMUI_HORIZONTAL,s->width, s->height);
    ctermui_widget child1 = ctermui_widget_new(LEAF, 100/3 );
    ctermui_widget child2 = ctermui_widget_new(CTERMUI_VERTICAL, 100/3);
        ctermui_widget child5 = ctermui_widget_new(LEAF, 50);
        ctermui_widget child4 = ctermui_widget_new(LEAF, 50);
    ctermui_widget child3 = ctermui_widget_new(LEAF, 100/3);


    ctermui_widget_add_child(root, child1);
    ctermui_widget_add_child(root, child2);
        ctermui_widget_add_child(child2, child4);
        ctermui_widget_add_child(child2, child5);
    ctermui_widget_add_child(root, child3);

    ctermui_component c = ctermui_new_text(
        "Hello World",
        CTERMUI_BRIGHT_YELLOW,
        CTERMUI_BRIGHT_BLUE,
        CTERMUI_ALIGN_BOTTOM_CENTER
    );
    ctermui_widget_add_component(child5, c);
    ctermui_component c2 = ctermui_new_frame(
        CTERMUI_BLUE,
        CTERMUI_BRIGHT_YELLOW
    );
    ctermui_widget_add_component(child5, c2);


    ctermui_component c4 = ctermui_new_frame(
        CTERMUI_BLUE,
        CTERMUI_BRIGHT_YELLOW
    );
    ctermui_widget_add_component(child4, c4);

    ctermui_component c3 = ctermui_new_background(
        CTERMUI_BLUE,
        root->width,
        root->height
    );
    ctermui_widget_add_component(root, c3);

    ctermui_screen_set_widget_root(s, root);
    ctermui_start(s);
}

void four_squares(){
    ctermui_screen_t s = ctermui_screen_new();
    ctermui_widget root = ctermui_widget_new_root(CTERMUI_HORIZONTAL,s->width, s->height);

    ctermui_widget child1 = ctermui_widget_new(CTERMUI_VERTICAL, 50);
    ctermui_widget child2 = ctermui_widget_new(CTERMUI_VERTICAL, 50);
    ctermui_widget_add_child(root, child1);
    ctermui_widget_add_child(root, child2);

    ctermui_widget child3 = ctermui_widget_new(LEAF, 50);
    ctermui_widget child4 = ctermui_widget_new(LEAF, 50);
    ctermui_widget_add_child(child1, child3);
    ctermui_widget_add_child(child1, child4);

    ctermui_widget child5 = ctermui_widget_new(LEAF, 50);
    ctermui_widget child6 = ctermui_widget_new(LEAF, 50);
    ctermui_widget_add_child(child2, child5);
    ctermui_widget_add_child(child2, child6);

    ctermui_component c = ctermui_new_frame(
        CTERMUI_BLUE,
        CTERMUI_BRIGHT_YELLOW
    );
    ctermui_widget_add_component(child3, c);

    ctermui_component c2 = ctermui_new_frame(
        CTERMUI_BLUE,
        CTERMUI_BRIGHT_YELLOW
    );
    ctermui_widget_add_component(child4, c2);

    ctermui_component c3 = ctermui_new_frame(
        CTERMUI_BLUE,
        CTERMUI_BRIGHT_YELLOW
    );
    ctermui_widget_add_component(child5, c3);

    ctermui_component c4 = ctermui_new_frame(
        CTERMUI_BLUE,
        CTERMUI_BRIGHT_YELLOW
    );
    ctermui_widget_add_component(child6, c4);

    ctermui_screen_set_widget_root(s, root);
    ctermui_start(s);
}
ctermui_screen_t s;
Button* btnList[4];
int bindex = 0;

void on_select(void* arg){
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

    ctermui_screen_keyboard_events_register(s->keyboard_events, 10, on_select, NULL);
    ctermui_screen_set_widget_root(s, root);
    ctermui_start(s);
}
int main() {
    ButtonSelectionExample();
    return 0;
}
