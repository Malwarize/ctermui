#include "ctermui/ctermui.h"
#include <string.h>


int main(){
    ctermui c = ctermui_new();
    ctermui_init(c);
    char* str = "Hello world!";
    ctermui_widget_t w2 = ctermui_widget_new(4, 4, strlen(str), 4);
    ctermui_text_box(c, w2, "Hello");
    start(c);
    return 0;
}   