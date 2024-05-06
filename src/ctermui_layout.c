#include "ctermui_layout.h"

ctermui_layout_t ctermui_layout_new_root(
        uint16_t type,
        size_t width,
        size_t height
) {
    ctermui_layout_t layout =
            malloc(sizeof(struct ctermui_layout));
    if (layout == NULL) {
        fprintf(
                stderr,
                "Error: could not allocate memory for layout\n"
        );
        exit(EXIT_FAILURE);
    }
    layout->type = type;
    layout->percentage = 100;
    layout->children_count = 0;
    layout->absolute_width = width;
    layout->absolute_height = height;
    strcpy(layout->id, "root");
    layout->absolute_x = 0;
    layout->absolute_y = 0;
    layout->absolute_width = width;
    layout->absolute_height = height;
    layout->component_count = 0;

    return layout;
}

ctermui_layout_t ctermui_layout_new(
        char *id,
        uint16_t type,
        uint16_t percentage
) {
    ctermui_layout_t layout =
            malloc(sizeof(struct ctermui_layout));
    if (layout == NULL) {
        fprintf(
                stderr,
                "Error: could not allocate memory for layout\n"
        );
        exit(EXIT_FAILURE);
    }
    layout->type = type;
    layout->percentage = percentage;
    layout->children_count = 0;
    layout->component_count = 0;
    strcpy(layout->id, id);
    return layout;
}

void ctermui_calculate_abs_position(
        ctermui_layout_t root_layout
) {
    if (root_layout->type == CTERMUI_HORIZONTAL) {
        ctermui_layout_t prev_child = NULL;
        for (size_t i = 0; i < root_layout->children_count; ++i) {
            root_layout->children[i]->absolute_width =
                    root_layout->children[i]->percentage *
                    root_layout->absolute_width / 100;
            root_layout->children[i]->absolute_height =
                    root_layout->absolute_height;
            root_layout->children[i]->absolute_x =
                    (prev_child == NULL ? root_layout->absolute_x :
                     prev_child->absolute_x) +
                    (prev_child == NULL ? 0 :
                     prev_child->absolute_width);
            root_layout->children[i]->absolute_y =
                    root_layout->absolute_x;
            prev_child = root_layout->children[i];
        }
    } else if (root_layout->type == CTERMUI_VERTICAL) {
        ctermui_layout_t prev_child = NULL;
        for (size_t i = 0; i < root_layout->children_count; ++i) {
            root_layout->children[i]->absolute_width =
                    root_layout->absolute_width;
            root_layout->children[i]->absolute_height =
                    root_layout->children[i]->percentage *
                    root_layout->absolute_height / 100;
            root_layout->children[i]->absolute_x =
                    root_layout->absolute_x;
            root_layout->children[i]->absolute_y =
                    (prev_child == NULL ? root_layout->absolute_y :
                     prev_child->absolute_y) +
                    (prev_child == NULL ? 0 :
                     prev_child->absolute_height);
            prev_child = root_layout->children[i];
        }
    }
    for (size_t i = 0; i < root_layout->component_count; ++i) {
        ctermui_component_t c = root_layout->component[i];
        c->calculate_absolute_position(
                c,
                root_layout->absolute_x,
                root_layout->absolute_y,
                root_layout->absolute_width,
                root_layout->absolute_height
        );
    }

    for (size_t i = 0; i < root_layout->children_count; ++i) {
        ctermui_calculate_abs_position(
                root_layout->children[i]
        );
    }
}

int ctermui_layout_add_child(
        ctermui_layout_t parent,
        ctermui_layout_t child
) {
    parent->children[parent->children_count++] = child;
    return 0;
}

int ctermui_layout_add_component(
        ctermui_layout_t layout,
        ctermui_component_t c
) {
    layout->component[layout->component_count++] = c;
    c->parent = layout;
    return 0;
}

ctermui_layout_t ctermui_layout_find(
        ctermui_layout_t root,
        char *id
) {
    if (strcmp(root->id, id) == 0) {
        return root;
    }
    for (size_t i = 0; i < root->children_count; ++i) {
        ctermui_layout_t w =
                ctermui_layout_find(root->children[i], id);
        if (w != NULL) {
            return w;
        }
    }
    return NULL;
}

ctermui_component_t ctermui_layout_find_component(
        ctermui_layout_t layout, char *id
) {
    if (layout == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < layout->component_count; i++) {
        if (strcmp(layout->component[i]->id, id) == 0) {
            return layout->component[i];
        }
    }
    return NULL;
}
