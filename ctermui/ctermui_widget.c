#include "ctermui_widget.h"

ctermui_widget ctermui_widget_new_root(uint16_t type, int width, int height) {
    ctermui_widget widget = malloc(sizeof(struct ctermui_widget));
    if(widget == NULL){
        fprintf(stderr, "Error: could not allocate memory for widget\n");
    }
    widget->type = type;
    widget->percentage = 100;
    widget->children_count = 0;
    widget->width = width;
    widget->height = height;
    strcmp(widget->id, "root");
    widget->absolute_x = 0;
    widget->absolute_y = 0;
    widget->absolute_width = width;
    widget->absolute_height = height;
    return widget;
}

ctermui_widget ctermui_widget_new(char* id, uint16_t type, uint16_t percentage) {
    ctermui_widget widget = malloc(sizeof(struct ctermui_widget));
    if(widget == NULL){
        fprintf(stderr, "Error: could not allocate memory for widget\n");
    }
    widget->type = type;
    widget->percentage = percentage;
    widget->children_count = 0;
    strcpy(widget->id, id);
    return widget;
}

void ctermui_calculate_abs_position(ctermui_widget root_widget)
{
    if(root_widget->component == NULL){
        return;
    }

    if(root_widget->type == CTERMUI_HORIZONTAL){
        ctermui_widget prev_child = NULL;
        for (int i = 0; i < root_widget->children_count; ++i){
            root_widget->children[i]->absolute_width = root_widget->children[i]->percentage * root_widget->absolute_width / 100;
            root_widget->children[i]->absolute_height = root_widget->absolute_height;
            root_widget->children[i]->absolute_x = (prev_child == NULL?root_widget->absolute_x:prev_child->absolute_x) + (prev_child == NULL ? 0 : prev_child->absolute_width);
            root_widget->children[i]->absolute_y = root_widget->absolute_x;
            prev_child = root_widget->children[i];
        }
    }else if(root_widget->type == CTERMUI_VERTICAL){
        ctermui_widget prev_child = NULL;
        for (int i = 0; i < root_widget->children_count; ++i){
            root_widget->children[i]->absolute_width = root_widget->absolute_width;
            root_widget->children[i]->absolute_height = root_widget->children[i]->percentage * root_widget->absolute_height / 100;
            root_widget->children[i]->absolute_x = root_widget->absolute_x;
            root_widget->children[i]->absolute_y = (prev_child == NULL?root_widget->absolute_y:prev_child->absolute_y) + (prev_child == NULL ? 0 : prev_child->absolute_height);
            prev_child = root_widget->children[i];
        }
    }
    for (int i = 0; i < root_widget->component_count; ++i){
        ctermui_component c = root_widget->component[i];
        if(c->type == TEXT){
            int text_width = strlen(((Text*)c->core_component)->text);
            if(((Text*)c->core_component)->align == CTERMUI_ALIGN_CENTER){
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y + (root_widget->absolute_height - 1)/2;
            }else if(((Text*)c->core_component)->align == CTERMUI_ALIGN_LEFT_TOP){
                c->absolute_x = root_widget->absolute_x + 1;
                c->absolute_y = root_widget->absolute_y + 1;
            }else if(((Text*)c->core_component)->align == CTERMUI_ALIGN_RIGHT_TOP){
                c->absolute_x = root_widget->absolute_x + root_widget->absolute_width - text_width;
                c->absolute_y = root_widget->absolute_y + 1;
            }else if(((Text*)c->core_component)->align == CTERMUI_ALIGN_TOP){
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y;
            }else if(((Text*)c->core_component)->align == CTERMUI_ALIGN_LEFT_BOTTOM){
                c->absolute_x = root_widget->absolute_x;
                c->absolute_y = root_widget->absolute_y + root_widget->absolute_height - 1;
            }else if(((Text*)c->core_component)->align == CTERMUI_ALIGN_RIGHT_BOTTOM){
                c->absolute_x = root_widget->absolute_x + root_widget->absolute_width - text_width;
                c->absolute_y = root_widget->absolute_y + root_widget->absolute_height - 1;
            }else if(((Text*)c->core_component)->align == CTERMUI_ALIGN_BOTTOM){
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y + root_widget->absolute_height - 1;
            }else if(((Text*)c->core_component)->align == CTERMUI_ALIGN_LEFT_CENTER){
                c->absolute_x = root_widget->absolute_x + 1;
                c->absolute_y = root_widget->absolute_y + (root_widget->absolute_height - 1)/2;
            }else if(((Text*)c->core_component)->align == CTERMUI_ALIGN_RIGHT_CENTER){
                c->absolute_x = root_widget->absolute_x + root_widget->absolute_width - text_width;
                c->absolute_y = root_widget->absolute_y + (root_widget->absolute_height - 1)/2;
            }else if(((Text*)c->core_component)->align == CTERMUI_ALIGN_TOP_CENTER){
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y+1;
            }else if(((Text*)c->core_component)->align == CTERMUI_ALIGN_BOTTOM_CENTER){
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y + root_widget->absolute_height - 1;
            }
            else{
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y + (root_widget->absolute_height - 1)/2;
            }
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
        }else if(c->type == BUTTON){
            int text_width = strlen(((Button*)c->core_component)->text);
            //frame width text_width + 2
            //frame height 3
            if(((Button*)c->core_component)->align == CTERMUI_ALIGN_CENTER){
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y + (root_widget->absolute_height - 3)/2;
            }else if(((Button*)c->core_component)->align == CTERMUI_ALIGN_LEFT_TOP){
                c->absolute_x = root_widget->absolute_x ;
                c->absolute_y = root_widget->absolute_y ;
            }else if(((Button*)c->core_component)->align == CTERMUI_ALIGN_RIGHT_TOP){
                c->absolute_x = root_widget->absolute_x + root_widget->absolute_width - text_width - 2;
                c->absolute_y = root_widget->absolute_y ;
            }else if(((Button*)c->core_component)->align == CTERMUI_ALIGN_TOP){
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y;
            }else if(((Button*)c->core_component)->align == CTERMUI_ALIGN_LEFT_BOTTOM){
                c->absolute_x = root_widget->absolute_x;
                c->absolute_y = root_widget->absolute_y + root_widget->absolute_height - 3;
            }else if(((Button*)c->core_component)->align == CTERMUI_ALIGN_RIGHT_BOTTOM){
                c->absolute_x = root_widget->absolute_x + root_widget->absolute_width - text_width - 2;
                c->absolute_y = root_widget->absolute_y + root_widget->absolute_height - 3;
            }else if(((Button*)c->core_component)->align == CTERMUI_ALIGN_BOTTOM){
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y + root_widget->absolute_height - 3;
            }else if(((Button*)c->core_component)->align == CTERMUI_ALIGN_LEFT_CENTER){
                c->absolute_x = root_widget->absolute_x;
                c->absolute_y = root_widget->absolute_y + (root_widget->absolute_height - 3)/2;
            }else if(((Button*)c->core_component)->align == CTERMUI_ALIGN_RIGHT_CENTER){
                c->absolute_x = root_widget->absolute_x + root_widget->absolute_width - text_width - 2;
                c->absolute_y = root_widget->absolute_y + (root_widget->absolute_height - 3)/2;
            }else if(((Button*)c->core_component)->align == CTERMUI_ALIGN_TOP_CENTER){
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y;
            }else if(((Button*)c->core_component)->align == CTERMUI_ALIGN_BOTTOM_CENTER){
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y + root_widget->absolute_height - 3;
            }
            else{
                c->absolute_x = root_widget->absolute_x + (root_widget->absolute_width - text_width)/2;
                c->absolute_y = root_widget->absolute_y + (root_widget->absolute_height - 3)/2;
            }
            c->absolute_width = text_width + 2;
            c->absolute_height = 3;
        }else if(c->type == CUSTOM){
            c->absolute_x = root_widget->absolute_x;
            c->absolute_y = root_widget->absolute_y;
            c->absolute_width = root_widget->absolute_width;
            c->absolute_height = root_widget->absolute_height;
        }
    }   

    for (int i = 0; i < root_widget->children_count; ++i){
        ctermui_calculate_abs_position(root_widget->children[i]);
    }

}

int ctermui_widget_add_child(ctermui_widget parent, ctermui_widget child) {
    parent->children[parent->children_count++] = child;
}

int ctermui_widget_add_component(ctermui_widget widget, ctermui_component c) {
    widget->component[widget->component_count++] = c;
    return 0;
}

ctermui_widget ctermui_widget_find(ctermui_widget root, char* id){
    if(strcmp(root->id, id) == 0){
        return root;
    }
    for (int i = 0; i < root->children_count; ++i){
        ctermui_widget w = ctermui_widget_find(root->children[i], id);
        if(w != NULL){
            return w;
        }
    }
    return NULL;
}

ctermui_component ctermui_widget_find_component(ctermui_widget widget, char* id)
{
    for (size_t i = 0; i < widget->component_count; i++)
    {
        if(strcmp(widget->component[i]->id, id) == 0){
            return widget->component[i];
        }
    }
    return NULL;
}