// #include "ctermui_widget.h"

// ctermui_widget ctermui_widget_new_root(uint16_t type, int width, int height) {
//     ctermui_widget widget = malloc(sizeof(struct ctermui_widget));
//     if(widget == NULL){
//         fprintf(stderr, "Error: could not allocate memory for widget\n");
//     }
//     widget->type = type;
//     widget->percentage = 100;
//     widget->cc = 0;
//     widget->x = 0;
//     widget->y = 0;
//     widget->width = width;
//     widget->height = height;

//     widget->absolute_x = 0;
//     widget->absolute_y = 0;
//     widget->absolute_width = width;
//     widget->absolute_height = height;
//     return widget;
// }

// ctermui_widget ctermui_widget_new(uint16_t type, uint16_t percentage) {
//     ctermui_widget widget = malloc(sizeof(struct ctermui_widget));
//     if(widget == NULL){
//         fprintf(stderr, "Error: could not allocate memory for widget\n");
//     }
//     widget->type = type;
//     widget->percentage = percentage;
//     widget->cc = 0;
//     return widget;
// }

// void ctermui_calculate_abs_position(ctermui_widget root_widget){
//     if(root_widget->type == LEAF){
//         root_widget->absolute_x = root_widget->x;
//         root_widget->absolute_y = root_widget->y;
//         root_widget->absolute_width = root_widget->width;
//         root_widget->absolute_height = root_widget->height;
//     }else{
//         for (int i = 0; i < root_widget->cc; ++i){
//             ctermui_calculate_abs_position(root_widget->children[i]);
//         }
//         if(root_widget->type == CTERMUI_HORIZONTAL){
//             int total_width = 0;
//             for (int i = 0; i < root_widget->cc; ++i){
//                 total_width += root_widget->children[i]->width;
//             }
//             int x = root_widget->x;
//             for (int i = 0; i < root_widget->cc; ++i){
//                 root_widget->children[i]->absolute_x = x;
//                 root_widget->children[i]->absolute_y = root_widget->y;
//                 root_widget->children[i]->absolute_width = root_widget->children[i]->width;
//                 root_widget->children[i]->absolute_height = root_widget->height;
//                 x += root_widget->children[i]->width;
//             }
//         }else if(root_widget->type == CTERMUI_VERTICAL){
//             int total_height = 0;
//             for (int i = 0; i < root_widget->cc; ++i){
//                 total_height += root_widget->children[i]->height;
//             }
//             int y = root_widget->y;
//             for (int i = 0; i < root_widget->cc; ++i){
//                 root_widget->children[i]->absolute_x = root_widget->x;
//                 root_widget->children[i]->absolute_y = y;
//                 root_widget->children[i]->absolute_width = root_widget->width;
//                 root_widget->children[i]->absolute_height = root_widget->children[i]->height;
//                 y += root_widget->children[i]->height;
//             }
//         }
//     }
// }

// int ctermui_widget_add_child(ctermui_widget parent, ctermui_widget child) {
    
//     //  if(parent->type == CTERMUI_HORIZONTAL){
//     //     if (parent->cc == 0) {
//     //         child->x = parent->x;
//     //         child->y = parent->y;
//     //         child->width = parent->width * child->percentage / 100;
//     //         child->height = parent->height;
//     //     } else {
//     //         ctermui_widget last_child = parent->children[parent->cc - 1];
//     //         child->x = last_child->x + last_child->width;
//     //         child->y = parent->y;
//     //         child->width = parent->width * child->percentage / 100;
//     //         child->height = parent->height;
//     //     } 
//     // }else if (parent->type == CTERMUI_VERTICAL){
//     //     if (parent->cc == 0) {
//     //         child->x = parent->x;
//     //         child->y = parent->y;
//     //         child->width = parent->width;
//     //         child->height = parent->height * child->percentage / 100;
//     //     } else {
//     //         ctermui_widget last_child = parent->children[parent->cc - 1];
//     //         child->x = parent->x;
//     //         child->y = last_child->y + last_child->height;
//     //         child->width = parent->width;
//     //         child->height = parent->height * child->percentage / 100;
//     //     } 
//     // }
//     // calculate relative position if(H) and perentage is 10% then width = parent->width * 10 / 100
//     //set relative position
//     if(parent->type == CTERMUI_HORIZONTAL){
//         child->width = child->percentage;
//     }
//     parent->children[parent->cc++] = child;
// }

// int ctermui_widget_add_component(ctermui_widget widget, ctermui_component c) {
//     //if Text center it
// //     if(c->type == TEXT){
// //         c->x = widget->x + (widget->width - c->width)/2;
// //         c->y = widget->y + (widget->height - c->height)/2;
// //     }
// //     if(c->type == FRAME){
// //         c->x = widget->x;
// //         c->y = widget->y;
// //     }
// //     widget->component[widget->component_count++] = c;
// //     return 0;
// //
//     widget->component[widget->component_count++] = c;
//     return 0;
// }

#include "ctermui_widget.h"
// 1. build the tree each widget has percentage and type
// 2. calculate the absolute position of each widget
/*
|root H
|
|-child1 50% 
|
|-child2 50%

calculate the absolute position of each widget
check the orientation of the parent then calculate the absolute position of the children
*/


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

    widget->absolute_x = 0;
    widget->absolute_y = 0;
    widget->absolute_width = width;
    widget->absolute_height = height;
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

void ctermui_calculate_abs_position(ctermui_widget root_widget)
{
    if(root_widget->component == NULL){
        return;
    }

    if(root_widget->type == CTERMUI_HORIZONTAL){
        ctermui_widget prev_child = NULL;
        for (int i = 0; i < root_widget->cc; ++i){
            root_widget->children[i]->absolute_width = root_widget->children[i]->percentage * root_widget->absolute_width / 100;
            root_widget->children[i]->absolute_height = root_widget->absolute_height;
            root_widget->children[i]->absolute_x = root_widget->absolute_x + (prev_child == NULL ? 0 : prev_child->absolute_width);
            root_widget->children[i]->absolute_y = root_widget->absolute_x;
            prev_child = root_widget->children[i];
        }
    }else if(root_widget->type == CTERMUI_VERTICAL){
        ctermui_widget prev_child = NULL;
        for (int i = 0; i < root_widget->cc; ++i){
            root_widget->children[i]->absolute_width = root_widget->absolute_width;
            root_widget->children[i]->absolute_height = root_widget->children[i]->percentage * root_widget->absolute_height / 100;
            root_widget->children[i]->absolute_x = root_widget->absolute_x;
            root_widget->children[i]->absolute_y = root_widget->absolute_y + (prev_child == NULL ? 0 : prev_child->absolute_height);
            prev_child = root_widget->children[i];
        }
    }
    for (int i = 0; i < root_widget->component_count; ++i){
        ctermui_component c = root_widget->component[i];
        if(c->type == TEXT){
            c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - c->absolute_width)/2;
            c->absolute_y = root_widget->absolute_y + (root_widget->absolute_height - c->absolute_height)/2;
        }else if(c->type == FRAME){
            c->absolute_x = root_widget->absolute_x;
            c->absolute_y = root_widget->absolute_y;
            c->absolute_width = root_widget->absolute_width;
            c->absolute_height = root_widget->absolute_height;

        }else if(c->type == BACKGROUND){
            c->absolute_x = root_widget->absolute_x;
            c->absolute_y = root_widget->absolute_y;
            c->absolute_width = root_widget->absolute_width;
            c->absolute_height = root_widget->absolute_height;
        }
    }

    for (int i = 0; i < root_widget->cc; ++i){
        ctermui_calculate_abs_position(root_widget->children[i]);
    }

}

//add child to parent 
int ctermui_widget_add_child(ctermui_widget parent, ctermui_widget child) {
    parent->children[parent->cc++] = child;
}

int ctermui_widget_add_component(ctermui_widget widget, ctermui_component c) {
    widget->component[widget->component_count++] = c;
    return 0;
}
