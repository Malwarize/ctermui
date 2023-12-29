#ifndef CTERMUI_EVENTS_H
#define CTERMUI_EVENTS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum CTERMUI_KEY {
    CTERMUI_KEY_UP = 65,
    CTERMUI_KEY_DOWN = 66,
    CTERMUI_KEY_RIGHT = 67,
    CTERMUI_KEY_LEFT = 68,
    CTERMUI_KEY_ENTER = 10,
    CTERMUI_KEY_SPACE = 32,
    CTERMUI_KEY_ESC = 27,
    CTERMUI_KEY_BACKSPACE = 127,
    CTERMUI_KEY_TAB = 9,
    ALL_KEYS = 0
};


typedef struct ctermui_screen_keyboard_event {
    char key;

    void (*callback)(void *);

    void *arg;
} *ctermui_screen_keyboard_event_t;

typedef struct ctermui_screen_keyboard_events {
    ctermui_screen_keyboard_event_t events[100];
    uint32_t ec;
} *ctermui_screen_keyboard_events_t;

ctermui_screen_keyboard_events_t
ctermui_screen_keyboard_events_new();

void ctermui_screen_keyboard_events_free(
        ctermui_screen_keyboard_events_t events
                                        );

void ctermui_screen_keyboard_events_register(
        ctermui_screen_keyboard_events_t events,
        char key,
        void (*callback)(void *),
        void *arg
                                            );

void ctermui_screen_keyboard_events_unregister(
        ctermui_screen_keyboard_events_t events, char key
                                              );

void ctermui_screen_keyboard_events_handle(
        ctermui_screen_keyboard_events_t events, char key
                                          );

#endif
