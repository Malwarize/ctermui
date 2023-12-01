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
        CTERMUI_BRIGHT_BLUE
    );
    ctermui_widget_add_component(child5, c);
    ctermui_component c2 = ctermui_new_frame(
        CTERMUI_BLUE,
        CTERMUI_BRIGHT_YELLOW,
        child5->width,
        child5->height
    );
    ctermui_widget_add_component(child5, c2);


    ctermui_component c4 = ctermui_new_frame(
        CTERMUI_BLUE,
        CTERMUI_BRIGHT_YELLOW,
        child4->width,
        child4->height
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


void SomeSquares(){
    // 10 x 10 squares 
    // devide the screen into 10 x 10 squares
    ctermui_screen_t s = ctermui_screen_new();
    ctermui_widget root = ctermui_widget_new_root(CTERMUI_HORIZONTAL,s->width, s->height);

    for (int i = 0; i < 10; ++i)
    {
        ctermui_widget child = ctermui_widget_new(CTERMUI_VERTICAL, 100/10);
        ctermui_widget_add_child(root, child);
        for (int j = 0; j < 10; ++j)
        {
            ctermui_widget child2 = ctermui_widget_new(LEAF, 100/10);
            ctermui_widget_add_child(child, child2);

            ctermui_component c = ctermui_new_frame(
                CTERMUI_BLUE,
                CTERMUI_BRIGHT_YELLOW,
                child2->width,
                child2->height
            );
            ctermui_widget_add_component(child2, c);
        }
    }

    ctermui_screen_set_widget_root(s, root);
    ctermui_start(s);
}



int main() {
    return 0;
}
