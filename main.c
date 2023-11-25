#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "ctermui/ctermui.h"




void callback_on_resize(int sig){
    printf("Window resized!\n");
}

int main(){
    ctermui c = ctermui_new();
    ctermui_init(c);
    ctermui_display_buffer(c);
    ctermui_line
    ctermui_line(c, 2, 0, 10, '|', 1);
    ctermui_set_string(c, 1, 0, "Hello World!");
    event_loop(c);
    return 0;
}   