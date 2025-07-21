#include "ctermui_events.h"

/**
 * @brief Allocate and initialize a new keyboard events structure.
 *
 * @return Pointer to the newly allocated keyboard events structure.
 * @note The caller is responsible for freeing the structure with ctermui_screen_keyboard_events_free.
 */
ctermui_screen_keyboard_events_t ctermui_screen_keyboard_events_new() {
  ctermui_screen_keyboard_events_t events =
      malloc(sizeof(struct ctermui_screen_keyboard_events));
  events->ec = 0;
  return events;
}

/**
 * @brief Free a keyboard events structure.
 *
 * @param events Pointer to the keyboard events structure to free.
 */
void ctermui_screen_keyboard_events_free(
    ctermui_screen_keyboard_events_t events) {
  free(events);
}

/**
 * @brief Register a callback for a specific key event.
 *
 * @param events Keyboard events structure.
 * @param key The key to register the callback for.
 * @param callback The function to call when the key is pressed.
 * @param arg Argument to pass to the callback function.
 * @note Supports up to 100 events. Exits on allocation failure.
 */
void ctermui_screen_keyboard_events_register(
    ctermui_screen_keyboard_events_t events, char key, void (*callback)(void *),
    void *arg) {
  if (events->ec >= 100) {
    fprintf(stderr,
            "ctermui_screen_keyboard_events_register: events->ec >= 100\n");
  }
  ctermui_screen_keyboard_event_t event =
      malloc(sizeof(struct ctermui_screen_keyboard_event));
  if (event == NULL) {
    fprintf(stderr, "ctermui_screen_keyboard_events_register: event == NULL\n");
    exit(EXIT_FAILURE);
  }
  event->key = key;
  event->callback = callback;
  event->arg = arg;
  events->events[events->ec++] = event;
}

/**
 * @brief Unregister a callback for a specific key event.
 *
 * @param events Keyboard events structure.
 * @param key The key to unregister.
 * @note Frees the memory for the event and shifts the remaining events.
 */
void ctermui_screen_keyboard_events_unregister(
    ctermui_screen_keyboard_events_t events, char key) {
  for (uint32_t i = 0; i < events->ec; i++) {
    if (events->events[i]->key == key) {
      free(events->events[i]);
      events->events[i] = events->events[--events->ec];
      events->events[events->ec] = NULL;

      // shift the rest of the array to the left
      for (uint32_t j = i; j < events->ec; j++) {
        events->events[j] = events->events[j + 1];
      }
      events->ec -= 1;
      return;
    }
  }
}

/**
 * @brief Handle a key event by calling the registered callback(s).
 *
 * @param events Keyboard events structure.
 * @param key The key that was pressed.
 * @note Calls all callbacks registered for the given key.
 */
void ctermui_screen_keyboard_events_handle(
    ctermui_screen_keyboard_events_t events, char key) {
  for (uint32_t i = 0; i < events->ec; i++) {
    if (events->events[i]->key == key) {
      events->events[i]->callback(events->events[i]->arg);
    }
  }
}
