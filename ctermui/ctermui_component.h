#ifndef CTERMUI_COMPONENT_H
#define CTERMUI_COMPONENT_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum CTYPES {
    TEXT,
    BUTTON,
};

typedef struct ctermui_component {
    uint16_t type;
    int x;
    int y;
    int width;
    int height;

    void* core_component;
} *ctermui_component;

typedef struct button {
    char text[100];
    void (*on_click)(void);
} Button;

typedef struct text {
    char text[100];
} Text;

ctermui_component ctermui_new_button(char* text, void (*on_click)(void));
ctermui_component ctermui_new_text(char* text);

#endif 