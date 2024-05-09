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
    layout->width = width;
    layout->height = height;
    strcpy(layout->id, "root");
    layout->x = 0;
    layout->y = 0;
    layout->width = width;
    layout->height = height;
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
            root_layout->children[i]->width =
                    root_layout->children[i]->percentage *
                    root_layout->width / 100;
            root_layout->children[i]->height =
                    root_layout->height;
            root_layout->children[i]->x =
                    (prev_child == NULL ? root_layout->x :
                     prev_child->x) +
                    (prev_child == NULL ? 0 :
                     prev_child->width);
            root_layout->children[i]->y =
                    root_layout->x;
            prev_child = root_layout->children[i];
        }
    } else if (root_layout->type == CTERMUI_VERTICAL) {
        ctermui_layout_t prev_child = NULL;
        for (size_t i = 0; i < root_layout->children_count; ++i) {
            root_layout->children[i]->width =
                    root_layout->width;
            root_layout->children[i]->height =
                    root_layout->children[i]->percentage *
                    root_layout->height / 100;
            root_layout->children[i]->x =
                    root_layout->x;
            root_layout->children[i]->y =
                    (prev_child == NULL ? root_layout->y :
                     prev_child->y) +
                    (prev_child == NULL ? 0 :
                     prev_child->height);
            prev_child = root_layout->children[i];
        }
    }
    for (size_t i = 0; i < root_layout->component_count; ++i) {
        ctermui_component_t c = root_layout->components[i];
        c->calculate_absolute_position(
                c,
                root_layout->x,
                root_layout->y,
                root_layout->width,
                root_layout->height
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
    layout->components[layout->component_count++] = c;
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
        if (strcmp(layout->components[i]->id, id) == 0) {
            return layout->components[i];
        }
    }
    return NULL;
}
