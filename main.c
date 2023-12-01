#include "ctermui/ctermui_screen.h"
#include <stdio.h>

int main() {
    ctermui_screen_t s = ctermui_screen_new();

    ctermui_widget root = ctermui_widget_new_root(CTERMUI_HORIZONTAL,s->width, s->height);
    ctermui_widget child1 = ctermui_widget_new(LEAF, 100/3 );
    ctermui_widget child2 = ctermui_widget_new(CTERMUI_VERTICAL, 100/3);
        ctermui_widget child4 = ctermui_widget_new(LEAF, 50);
        ctermui_widget child5 = ctermui_widget_new(LEAF, 50);
    ctermui_widget child3 = ctermui_widget_new(LEAF, 100/3);


    ctermui_widget_add_child(root, child1);
    ctermui_widget_add_child(root, child2);
        ctermui_widget_add_child(child2, child4);
        ctermui_widget_add_child(child2, child5);
    ctermui_widget_add_child(root, child3);

    ctermui_component c = ctermui_new_text("Hello World", CTERMUI_BLUE, CTERMUI_BRIGHT_YELLOW);
    ctermui_widget_add_component(child5, c);

    ctermui_screen_set_widget_root(s, root);
    ctermui_start(s);
    return 0;
}
