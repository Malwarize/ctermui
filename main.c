#include "ctermui/ctermui_screen.h"
#include <unistd.h>

void periodic(ctermui_screen_t* screen_p)
{
  ctermui_screen_t screen = *screen_p;
  ctermui_widget root = screen->root;
  ctermui_component text_input = root->component[0];
  if(screen->loop_count == 0) {
    TextInput *t = (TextInput*)text_input->core_component;
    t->selected = 1;
  }
  ctermui_screen_refresh_widget(screen, root);
}

int main()
{
  ctermui_screen_t screen = ctermui_screen_new();
  ctermui_widget root = ctermui_widget_new_root(
    CTERMUI_HORIZONTAL, screen->width, screen->height);
  ctermui_component text_input = ctermui_new_text_input("text", CTERMUI_BLUE, CTERMUI_CYAN, root->width, 1,screen->keyboard_events);
  ctermui_widget_add_component(root, text_input);
  ctermui_screen_set_widget_root(screen, root);
  ctermui_screen_loop_start(screen, periodic, 1000);
}
