#include "ctermui/ctermui_screen.h"
#include <time.h>
#include <math.h>

#define SIZE 50

void generate_data(float xvalues[], float yvalues[]) {
    srand(time(NULL));
    for (int i = 0; i < SIZE; ++i) {
        xvalues[i] = i;
        yvalues[i] = sin(i);
    }
}

void periodic(ctermui_screen_t *screen_p) {
    ctermui_screen_t screen = *screen_p;
    ctermui_layout_t root = screen->root;
    ctermui_component_t c = root->component[0];
    ScatterPlot *plt = (ScatterPlot *) c->core_component;
    float xvalues[SIZE];
    float yvalues[SIZE];
    if (screen->loop_count % 100 == 0) {

        for (int i = 0; i < SIZE; i++) {
            plt->xvalues[i]++;
            plt->yvalues[i] = sin(plt->xvalues[i]);
        }
        ctermui_screen_refresh_layout(screen, root);
    }

}


int main() {

    ctermui_screen_t screen = ctermui_screen_init();
    ctermui_layout_t root = ctermui_layout_new_root(
            CTERMUI_HORIZONTAL, screen->width, screen->height
                                                   );

    float xvalues[SIZE];
    float yvalues[SIZE];
    generate_data(xvalues, yvalues);
    ctermui_component_t plt = ctermui_new_scatter_plot(
            "scatter_plot",
            xvalues,
            yvalues,
            SIZE,
            CTERMUI_BRIGHT_CYAN,
            CTERMUI_EMPTY,
            CTERMUI_BRIGHT_RED,
            '*',
            1
                                                      );
    ctermui_layout_add_component(root, plt);
    ctermui_screen_set_layout_root(screen, root);
    ctermui_screen_loop_start(screen, periodic, 10000);
}
