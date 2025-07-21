# CTERMUI

A simple, modular terminal UI library for C.

![ctermui_logo](https://github.com/Malwarize/ctermui/assets/130087473/ca6b76b4-a2ec-4c2b-8c50-ba7c6fad8eca)

## Features
- Build interactive terminal user interfaces in C
- Layout system (horizontal/vertical, nested)
- Components: buttons, labels, progress bars, bar charts, scatter plots, forms, and more
- Custom component support
- Keyboard event handling

## Installation

Clone the repository and build with `make`:
```sh
make
```

## Getting Started

1. **Initialize the screen:**
```c
ctermui_screen_t screen = ctermui_screen_init();
```

2. **Create a root layout:**
```c
ctermui_layout_t root = ctermui_layout_new_root(
    CTERMUI_HORIZONTAL, screen->width, screen->height
);
```

3. **Create and add a component:**
```c
float xvalues[SIZE];
float yvalues[SIZE];
generate_data(xvalues, yvalues);
ctermui_component_t plt = ctermui_new_scatter_plot(
    "scatter_plot",
    xvalues,
    yvalues,
    SIZE,
    CTERMUI_BRIGHT_CYAN,
    CTERMUI_EMPTY,
    CTERMUI_BRIGHT_RED,
    '*',
    1
);
ctermui_layout_add_component(root, plt);
ctermui_screen_set_layout_root(screen, root);
```

4. **Start the main loop:**
```c
ctermui_screen_loop_start(screen, periodic, n);
```

5. **Example periodic function:**
```c
void periodic(ctermui_screen_t *screen_p) {
    ctermui_screen_t screen = *screen_p;
    ctermui_layout_t root = screen->root;
    ctermui_component_t c = root->components[0];
    ScatterPlot *plt = (ScatterPlot *) c->core_component;
    if (screen->loop_count % 100 == 0) {
        for (int i = 0; i < SIZE; i++) {
            plt->xvalues[i]++;
            plt->yvalues[i] = sin(plt->xvalues[i]);
        }
        ctermui_screen_refresh_layout(screen, root);
    }
}
```

## Examples
- [Button navigation](examples/button_navigation.c)
- [System usage](examples/grabage_linux_sys_usage.c)
- [Star Wars](examples/star_wars.c)
- [Scatter plot](examples/plot_sin.c)
- [Progress bar](examples/progress_bar.c)
- [Bar chart](examples/two_barcharts.c)

---

For more details, see the example programs in the `examples/` directory.

