# CTERMUI 
terminal UI library in C

![ctermui_logo](https://github.com/Malwarize/ctermui/assets/130087473/ca6b76b4-a2ec-4c2b-8c50-ba7c6fad8eca)

### get started
you have to initiate screen struct
```C
ctermui_screen_t screen = ctermui_screen_init();
```
we to get root layout ready also
```C
ctermui_layout_t root = ctermui_layout_new_root(
  CTERMUI_HORIZONTAL, screen->width, screen->height
);
```
you have to add component to layout
```C
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
```
after the screen, component and layout we have to attach them
```C
ctermui_layout_add_component(root, plt);
ctermui_screen_set_layout_root(screen, root);
```

start the screen loop 
```C
ctermui_screen_loop_start(screen, periodic, n);
```
the periodic function is function called every n ms
```C
void periodic(ctermui_screen_t *screen_p) {
    ctermui_screen_t screen = *screen_p;
    ctermui_layout_t root = screen->root;
    ctermui_component_t c = root->component[0];
    ScatterPlot *plt = (ScatterPlot *) c->core_component;
    float xvalues[SIZE];
    float yvalues[SIZE];
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
[button navigation](https://github.com/Malwarize/ctermui/blob/main/examples/button_navigation.c)

https://github.com/Malwarize/ctermui/assets/130087473/3a21eb8b-df69-41c1-b450-7baf858c5873

[system usage](https://github.com/Malwarize/ctermui/blob/main/examples/grabage_linux_sys_usage.c)

https://github.com/Malwarize/ctermui/assets/130087473/8f6ccb76-5e55-47ed-b0d3-6ed9c7888875

[star wars](https://github.com/Malwarize/ctermui/blob/main/examples/star_wars.c)

https://github.com/Malwarize/ctermui/assets/130087473/06fa3bbe-b2e3-407a-896c-ad86d3584d9b

[scatter plot](https://github.com/Malwarize/ctermui/blob/main/examples/plot_sin.c)

https://github.com/Malwarize/ctermui/assets/130087473/2d421ecc-3177-4832-8fa9-886e35ca0544

[progress bar](https://github.com/Malwarize/ctermui/blob/main/examples/progress_bar.c)

https://github.com/Malwarize/ctermui/assets/130087473/7eec2f17-8947-40ba-887f-88ff283941b5

[bar chart](https://github.com/Malwarize/ctermui/blob/main/examples/two_barcharts.c)

https://github.com/Malwarize/ctermui/assets/130087473/dcb3dc08-9ed4-46ec-950e-d85023465256

