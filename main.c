#include "ctermui/ctermui.h"
#include <string.h>


int main(){
    ctermui c = ctermui_new();
    ctermui_init(c);
    // ctermui_widget_relative_t w = ctermui_widget_relative_new(1, 1, 99, 99);
    // ctermui_widget_relative(c, w);
    char** new_buffer = malloc(sizeof(char*) * c->size.ws_row);
    ctermui_widget_t w = ctermui_widget_new(1, 1, 10, 20);
    ctermui_widget(c, w);
    start(c);
    return 0;
}   