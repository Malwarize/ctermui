#include "ctermui/ctermui_screen.h"
#include <stdio.h>
#include <unistd.h>

ctermui_screen_t s;
Button* btnList[4];
int bindex = 0;

void on_click(void* arg)
{
  Button* btn = btnList[bindex];
  strcpy(btn->text, "clicked");
  //get the widget of the button
  ctermui_widget w = ctermui_widget_find(s->root, "child1");
}

void select_button(Button* btn)
{
  btn->text_color = CTERMUI_BRIGHT_YELLOW;
  btn->bg_color = CTERMUI_RED;
}

void unselect_button(Button* btn)
{
  btn->text_color = CTERMUI_BLACK;
  btn->bg_color = CTERMUI_BRIGHT_YELLOW;
}

void on_select_arrow_up(void* arg)
{
  unselect_button(btnList[bindex]);
  bindex = (bindex - 1) % 4;
  if (bindex < 0) {
    bindex = 3;
  }
  select_button(btnList[bindex]);
}

void on_select_arrow_down(void* arg)
{
  unselect_button(btnList[bindex]);
  bindex = (bindex + 1) % 4;
  select_button(btnList[bindex]);
}

void periodic(ctermui_screen_t* sp)
{
  ctermui_screen_t s = *sp;
  ctermui_widget w1 =
    ctermui_widget_find(s->root, "child1");
  ctermui_component c1 =
    ctermui_widget_find_component(w1, "btn1");
  Button* btn = (Button*)c1->core_component;
  sprintf(btn->text, "%d", s->loop_count);
  btn->bg_color = CTERMUI_BRIGHT_CYAN;
  ctermui_screen_refresh_widget(s, w1);
}

void ButtonSelectionExample()
{
  s = ctermui_screen_new();
  ctermui_widget root = ctermui_widget_new_root(
    CTERMUI_VERTICAL, s->width, s->height);
  for (size_t i = 0; i < 4; i++) {
    char id[100];
    sprintf(id, "child%zu", i);
    ctermui_widget child1 =
      ctermui_widget_new(id, CTERMUI_HORIZONTAL, 20);
    char id1[100];
    sprintf(id1, "btn%zu", i);
    ctermui_component btn1 =
      ctermui_new_button(id1,
                         "select me",
                         CTERMUI_ALIGN_CENTER,
                         CTERMUI_BLACK,
                         CTERMUI_BRIGHT_YELLOW);
    ctermui_widget_add_component(child1, btn1);
    char id2[100];
    sprintf(id2, "frame%zu", i);
    ctermui_widget_add_component(
      child1,
      ctermui_new_frame(
        id2, CTERMUI_BRIGHT_YELLOW, CTERMUI_BRIGHT_YELLOW));
    ctermui_widget_add_child(root, child1);
    btnList[i] = (Button*)btn1->core_component;
  }
  ctermui_screen_keyboard_events_register(
    s->keyboard_events, 65, on_select_arrow_up, NULL);
  ctermui_screen_keyboard_events_register(
    s->keyboard_events, 66, on_select_arrow_down, NULL);

  ctermui_screen_keyboard_events_register(
    s->keyboard_events, 10, on_click, NULL);
  ctermui_screen_set_widget_root(s, root);
  ctermui_screen_loop_start(s, periodic, 10000);
}

int main()
{
  ButtonSelectionExample();
  return 0;
}
