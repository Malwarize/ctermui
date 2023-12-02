#ifndef CTERMUI_EVENTS_H
#define CTERMUI_EVENTS_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct ctermui_screen_keyboard_event{
    char key;
    void (*callback)(void*);
    void* arg;
} *ctermui_screen_keyboard_event_t;

typedef struct ctermui_screen_keyboard_events{
    ctermui_screen_keyboard_event_t events[100];
    uint32_t ec;
}*ctermui_screen_keyboard_events_t;

ctermui_screen_keyboard_events_t ctermui_screen_keyboard_events_new();
void ctermui_screen_keyboard_events_free(ctermui_screen_keyboard_events_t events);
void ctermui_screen_keyboard_events_register(ctermui_screen_keyboard_events_t events, char key, void (*callback)(void*), void* arg);
void ctermui_screen_keyboard_events_unregister(ctermui_screen_keyboard_events_t events, char key);
void ctermui_screen_keyboard_events_handle(ctermui_screen_keyboard_events_t events, char key);
#endif