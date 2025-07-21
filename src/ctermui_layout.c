#include "ctermui_layout.h"

/**
 * @brief Create a new root layout for the UI.
 *
 * This function allocates and initializes a root layout structure with the given orientation and size.
 * The root layout always has 100% percentage and is positioned at (0,0).
 *
 * @param type Layout orientation (CTERMUI_HORIZONTAL or CTERMUI_VERTICAL).
 * @param width Width of the root layout in characters.
 * @param height Height of the root layout in characters.
 * @return Pointer to the newly allocated layout structure. Exits on allocation failure.
 * @note The caller is responsible for freeing the layout (if a free function exists).
 */
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

/**
 * @brief Create a new (non-root) layout.
 *
 * This function allocates and initializes a layout structure with the given ID, orientation, and percentage size.
 *
 * @param id String identifier for the layout (copied internally).
 * @param type Layout orientation (CTERMUI_HORIZONTAL or CTERMUI_VERTICAL).
 * @param percentage Size of this layout as a percentage of its parent.
 * @return Pointer to the newly allocated layout structure. Exits on allocation failure.
 * @note The caller is responsible for freeing the layout (if a free function exists).
 */
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

/**
 * @brief Recursively calculate absolute positions and sizes for all layouts and components.
 *
 * This function updates the x, y, width, and height fields of all child layouts and components
 * based on their parent's dimensions and their own percentage or alignment settings.
 *
 * @param root_layout The root layout to start calculation from.
 * @note This should be called after modifying the layout tree or resizing the screen.
 */
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

/**
 * @brief Add a child layout to a parent layout.
 *
 * @param parent The parent layout.
 * @param child The child layout to add.
 * @return 0 on success.
 * @note No bounds checking is performed; ensure you do not exceed the maximum children.
 */
int ctermui_layout_add_child(
        ctermui_layout_t parent,
        ctermui_layout_t child
) {
    parent->children[parent->children_count++] = child;
    return 0;
}

/**
 * @brief Add a UI component to a layout.
 *
 * @param layout The layout to add the component to.
 * @param c The component to add.
 * @return 0 on success.
 * @note No bounds checking is performed; ensure you do not exceed the maximum components.
 *       The component's parent pointer is set to this layout.
 */
int ctermui_layout_add_component(
        ctermui_layout_t layout,
        ctermui_component_t c
) {
    layout->components[layout->component_count++] = c;
    c->parent = layout;
    return 0;
}

/**
 * @brief Recursively search for a layout by ID in the layout tree.
 *
 * @param root The root layout to start searching from.
 * @param id The string ID to search for.
 * @return Pointer to the layout with the matching ID, or NULL if not found.
 */
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

/**
 * @brief Search for a component by ID within a layout.
 *
 * @param layout The layout to search in.
 * @param id The string ID of the component.
 * @return Pointer to the component with the matching ID, or NULL if not found.
 */
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
