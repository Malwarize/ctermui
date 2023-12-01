#include "ctermui_widget.h"

ctermui_widget ctermui_widget_new_root(uint16_t type, int width, int height) {
    ctermui_widget widget = malloc(sizeof(struct ctermui_widget));
    if(widget == NULL){
        fprintf(stderr, "Error: could not allocate memory for widget\n");
    }
    widget->type = type;
    widget->percentage = 100;
    widget->cc = 0;
    widget->x = 0;
    widget->y = 0;
    widget->width = width;
    widget->height = height;
    return widget;
}

ctermui_widget ctermui_widget_new(uint16_t type, uint16_t percentage) {
    ctermui_widget widget = malloc(sizeof(struct ctermui_widget));
    if(widget == NULL){
        fprintf(stderr, "Error: could not allocate memory for widget\n");
    }
    widget->type = type;
    widget->percentage = percentage;
    widget->cc = 0;
    return widget;
}

int ctermui_widget_add_child(ctermui_widget parent, ctermui_widget child) {
    
     if(parent->type == CTERMUI_HORIZONTAL){
        if (parent->cc == 0) {
            child->x = parent->x;
            child->y = parent->y;
            child->width = parent->width * child->percentage / 100;
            child->height = parent->height;
        } else {
            ctermui_widget last_child = parent->children[parent->cc - 1];
            child->x = last_child->x + last_child->width;
            child->y = parent->y;
            child->width = parent->width * child->percentage / 100;
            child->height = parent->height;
        } 
    }else if (parent->type == CTERMUI_VERTICAL){
        if (parent->cc == 0) {
            child->x = parent->x;
            child->y = parent->y;
            child->width = parent->width;
            child->height = parent->height * child->percentage / 100;
        } else {
            ctermui_widget last_child = parent->children[parent->cc - 1];
            child->x = parent->x;
            child->y = last_child->y + last_child->height;
            child->width = parent->width;
            child->height = parent->height * child->percentage / 100;
        } 
    }
    parent->children[parent->cc++] = child;
}

int ctermui_widget_add_component(ctermui_widget widget, ctermui_component c) {
    if (widget->type != LEAF) {
        return -1;
    }
    c->x = widget->x;
    c->y = widget->y;
    c->width = widget->width;
    c->height = widget->height;
    widget->component = c;
    return 0;
}
