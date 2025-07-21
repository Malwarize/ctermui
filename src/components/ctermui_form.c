#include "ctermui_component.h"
#include "ctermui_pencil.h"
#include "ctermui_screen.h"

struct comp_key {
  ctermui_component_t c;
  size_t key;
};

/**
 * @brief Delete the last character from the text input.
 *
 * @param arg Pointer to the text input component.
 */
void ctemrui_text_input_delete(void *arg) {
  ctermui_component_t c = arg;
  char *text = ((TextInput *)c->core_component)->text;
  size_t text_length = strlen(text);
  if (text_length > 0) {
    text[text_length - 1] = '\0';
  }
}

/**
 * @brief Write a character to the text input component.
 *
 * @param args Pointer to a comp_key struct containing the component and key.
 */
void ctermui_text_input_write(void *args) {
  struct comp_key *ck = args;
  ctermui_component_t c = ck->c;
  size_t key = ck->key;
  TextInput *text_input = c->core_component;
  if (text_input->selected != 1) {
    return;
  }
  size_t text_length = strlen(text_input->text);
  if (text_length < c->width - 2) {
    text_input->text[text_length] = (char)key;
    text_input->text[text_length + 1] = '\0';
  }
}

/**
 * @brief Calculate the absolute position and size of a text input component.
 *
 * @param c Pointer to the text input component.
 * @param parent_x X coordinate of the parent layout.
 * @param parent_y Y coordinate of the parent layout.
 * @param parent_width Width of the parent layout.
 * @param parent_height Height of the parent layout.
 */
void ctermui_text_input_calculate_absolute_position(ctermui_component_t c,
                                                    size_t parent_x,
                                                    size_t parent_y,
                                                    size_t parent_width,
                                                    size_t parent_height) {
  TextInput *text_input = c->core_component;
  c->width =
      (size_t)((float)text_input->min_width / 100.0 * (int)parent_width);
  c->height =
      (size_t)((float)text_input->min_height / 100.0 * (int)parent_height);
  c->x = parent_x;
  c->y = parent_y;
}

/**
 * @brief Draw the text input component on the screen.
 *
 * @param s Pointer to the screen structure.
 * @param c Pointer to the text input component.
 */
void ctermui_text_input_draw(ctermui_screen_t s, ctermui_component_t c) {
  TextInput *text_input = c->core_component;
  size_t text_width = strlen(text_input->text);
  size_t frame_width = c->width;
  size_t frame_height = c->height;
  if (text_width >= frame_width) {
    frame_width = frame_width + (text_width - frame_width);
  }
  ctermui_pencil_solid_background(s->buffer, c->x, c->y, frame_width,
                                  frame_height, text_input->bg_color);
  ctermui_pencil_draw_text(s->buffer, c->x, c->y + (frame_height - 1) / 2,
                           text_input->text, text_input->text_color,
                           text_input->bg_color);
}

/**
 * @brief Create a new text input component.
 *
 * Allocates and initializes a new text input with the given parameters and registers keyboard events.
 *
 * @param id String identifier for the text input.
 * @param text_color Color of the text.
 * @param bg_color Background color.
 * @param min_width Minimum width as a percentage of parent.
 * @param min_height Minimum height as a percentage of parent.
 * @param events Keyboard events handler to register input callbacks.
 * @return Pointer to the new text input component. Exits on allocation failure.
 * @note Caller is responsible for freeing the component (if a free function exists).
 */
ctermui_component_t
ctermui_new_text_input(char *id, int8_t text_color, int8_t bg_color,
                       size_t min_width, size_t min_height,
                       ctermui_screen_keyboard_events_t events) {
  ctermui_component_t c = malloc(sizeof(struct ctermui_component));
  if (c == NULL) {
    fprintf(stderr,
            "Error: could not allocate memory for text input component\n");
    exit(EXIT_FAILURE);
  }
  TextInput *text_input_component = malloc(sizeof(TextInput));

  if (text_input_component == NULL) {
    fprintf(stderr,
            "Error: could not allocate memory for text input component\n");
    exit(EXIT_FAILURE);
  }

  text_input_component->text_color = text_color;
  text_input_component->bg_color = bg_color;
  text_input_component->selected = 0;
  text_input_component->min_width = min_width;
  text_input_component->min_height = min_height;
  c->core_component = text_input_component;
  c->type = TEXT_INPUT;
  strcpy(c->id, id);
  c->draw = ctermui_text_input_draw;
  c->calculate_absolute_position =
      ctermui_text_input_calculate_absolute_position;

  // Add to screen all keyboard events
  ctermui_screen_keyboard_events_register(events, 127,
                                          ctemrui_text_input_delete, c);

  for (size_t i = 32; i < 127; i++) {
    struct comp_key *ck = malloc(sizeof(struct comp_key));
    if (ck == NULL) {
      fprintf(stderr,
              "Error: could not allocate memory for text input component\n");
      exit(EXIT_FAILURE);
    }
    ck->c = c;
    ck->key = i;
    ctermui_screen_keyboard_events_register(events, i,
                                            ctermui_text_input_write, ck);
    // Todo: find a way to free ck
  }
  return c;
  // Todo: use the custom func
}
