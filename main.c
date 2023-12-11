#include "ctermui/ctermui_screen.h"
#include <unistd.h>

void periodic(ctermui_screen_t *screen_p){
    ctermui_screen_t screen = *screen_p;
    ctermui_widget root = screen->root;
    for(int i = 0; i < 10; i++){
        if(i % 2 == 0){
            continue;
        }
        char id[100];
        sprintf(id, "progress_bar_%d", i);
        ctermui_component progress_bar = ctermui_widget_find_component(ctermui_widget_find(root, id), id);
        ProgressBar *pb = (ProgressBar *)progress_bar->core_component;
        if(pb->progress == 100){
            pb->text = "Done!";
            ctermui_screen_refresh_widget(screen, root);
        }
        if(screen->loop_count % 10 == 0 && pb->progress < 100){
            pb->progress += 1;
            ctermui_screen_refresh_widget(screen, root);
        }
    }
}
int main() {
    /* ctermui_screen_t screen = ctermui_screen_new(); */
    /* ctermui_widget root = ctermui_widget_new_root(CTERMUI_VERTICAL, screen->width, screen->height); */
    /* ctermui_widget padding_h = ctermui_widget_new("padding_h", CTERMUI_HORIZONTAL, 10); */
    /* ctermui_widget progress_widget = ctermui_widget_new("progress_w", LEAF, 50); */

    /* char id[100]; */
    /* for(int i = 0; i < 10; i++){ */
    /*     sprintf(id, "progress_bar_%d", i); */
    /*     ctermui_component progress_bar = ctermui_new_progress_bar( */
    /*         id, */
    /*         CTERMUI_GREEN, */ 
    /*         CTERMUI_CYAN, */ 
    /*         100, */
    /*         0, */
    /*         "Loading...", */
    /*         CTERMUI_WHITE */
    /*     ); */
    /* } */
    /* //padding widget */
    /* ctermui_widget_add_component(progress_widget, progress_bar); */
    /* ctermui_widget_add_child(padding_h, progress_widget); */
    /* ctermui_widget_add_child(root, padding_h); */
    /* ctermui_screen_set_widget_root(screen, root); */
    /* ctermui_screen_loop_start(screen, periodic, 10000); */
    ctermui_screen_t screen = ctermui_screen_new();
    ctermui_widget root = ctermui_widget_new_root(CTERMUI_HORIZONTAL, screen->width, screen->height);
    ctermui_widget padding_h = ctermui_widget_new("padding_h", CTERMUI_VERTICAL, 50);
    for(int i = 0; i < 10; i++){
        if(i%2 == 0){
            ctermui_widget_add_child(padding_h, ctermui_widget_new("padding", LEAF, 10));
            continue;
        }
        char id[100];
        sprintf(id, "progress_bar_%d", i);
        ctermui_component progress_bar = ctermui_new_progress_bar(
            id,
            CTERMUI_GREEN, 
            CTERMUI_CYAN, 
            100, 
            i, 
            "Loading...", 
            CTERMUI_WHITE,
            CTERMUI_HORIZONTAL
        );
        ctermui_widget progress_widget = ctermui_widget_new(id, LEAF, 10);
        ctermui_widget_add_component(progress_widget, progress_bar);
        ctermui_widget_add_child(padding_h, progress_widget);
    }
    ctermui_widget_add_child(root, padding_h);
    ctermui_screen_set_widget_root(screen, root);
    ctermui_screen_loop_start(screen, periodic, 10000);
}
