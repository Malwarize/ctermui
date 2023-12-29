#include "ctermui/ctermui_screen.h"

void periodic(ctermui_screen_t *screen_p) {
    ctermui_screen_t screen = *screen_p;
    ctermui_layout_t root = screen->root;
    ctermui_component_t barchart = ctermui_layout_find_component(root->children[0], "barchart");
    ctermui_component_t barchart_2 = ctermui_layout_find_component(root->children[1], "barchart_2");

    BarChart *bc = (BarChart *) barchart->core_component;
    int values[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    if (screen->loop_count % 100 == 0) {
        for (int i = 0; i < 10; i++) {
            values[i] = rand() % 100;
        }
        ctermui_barchart_update_values(barchart, values, 10);
        ctermui_barchart_update_values(barchart_2, values, 10);
        ctermui_screen_refresh_layout(screen, root);
    }
}

int main() {
    ctermui_screen_t screen = ctermui_screen_init();
    ctermui_layout_t root = ctermui_layout_new_root(
            CTERMUI_HORIZONTAL, screen->width, screen->height
                                                   );


    ctermui_layout_t wiget_1 = ctermui_layout_new("left_layout", LEAF, 50);
    ctermui_layout_t wiget_2 = ctermui_layout_new("right_layout", LEAF, 50);

    int values[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    char labels[][100] = {"10", "20", "30", "40", "50", "60", "70", "80", "90", "100 "};
    ctermui_component_t barchart = ctemrui_new_barchart(
            "barchart", CTERMUI_MAGENTA, CTERMUI_EMPTY, 100, CTERMUI_HORIZONTAL, values, labels, 10, 1
                                                       );
    ctermui_component_t barchart_2 = ctemrui_new_barchart(
            "barchart_2", CTERMUI_RED, CTERMUI_EMPTY, 100, CTERMUI_VERTICAL, values, labels, 10, 1
                                                         );

    ctermui_layout_add_component(wiget_1, barchart);
    ctermui_layout_add_component(wiget_2, barchart_2);
    ctermui_layout_add_child(root, wiget_1);
    ctermui_layout_add_child(root, wiget_2);
    ctermui_screen_set_layout_root(screen, root);
    ctermui_screen_loop_start(screen, periodic, 10000);
}
