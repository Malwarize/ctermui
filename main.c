#include "ctermui/ctermui_screen.h"
#include <stdio.h>

int main() {
    ctermui_screen_t s = ctermui_screen_new();
    ctermui_screen_test(s);
    ctermui_start(s);
    return 0;
}
