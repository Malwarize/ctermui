#include  "ctermui_events.h"

ctermui_screen_keyboard_events_t ctermui_screen_keyboard_events_new(){
    ctermui_screen_keyboard_events_t events = malloc(sizeof(struct ctermui_screen_keyboard_events));
    events->ec = 0;
    return events;
}

void ctermui_screen_keyboard_events_free(ctermui_screen_keyboard_events_t events){
    free(events);
}
void ctermui_screen_keyboard_events_register(ctermui_screen_keyboard_events_t events, char key, void (*callback)(void*), void* arg){
    if(events->ec >= 100){
        fprintf(stderr, "ctermui_screen_keyboard_events_register: events->ec >= 100\n");
    }
    ctermui_screen_keyboard_event_t event = malloc(sizeof(struct ctermui_screen_keyboard_event));
    event->key = key;
    event->callback = callback;
    event->arg = arg;
    events->events[events->ec++] = event;
}
void ctermui_screen_keyboard_events_unregister(ctermui_screen_keyboard_events_t events, char key){
    for(uint32_t i = 0; i < events->ec; i++){
        if(events->events[i]->key == key){
            free(events->events[i]);
            events->events[i] = events->events[--events->ec];
            return;
        }
    }
}
void ctermui_screen_keyboard_events_handle(ctermui_screen_keyboard_events_t events, char key){
    for(uint32_t i = 0; i < events->ec; i++){
        if(events->events[i]->key == key){
            events->events[i]->callback(events->events[i]->arg);
            return;
        }
    }
}