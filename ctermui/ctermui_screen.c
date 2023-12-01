#include "ctermui_screen.h"
#include <unistd.h>

winsize __get_term_size()
{
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w;
}

const char EMPTY_CHAR = ' ';

void __ctermui_screen_clean_term()
{
    printf("\033[?25l");
    system("clear");
}

void __restore_term()
{
    printf("\033[?25h");
}
ctermui_screen_t ctermui_screen_new()
{
    ctermui_screen_t screen = (ctermui_screen_t)malloc(sizeof(struct ctermui_screen));
    if (!screen)
    {
        fprintf(stderr, "Error: could not allocate memory for screen\n");
        exit(EXIT_FAILURE);
    }
    winsize w = __get_term_size();
    screen->width = w.ws_col;
    screen->height = w.ws_row;

    screen->buffer = (char ***)malloc(screen->width * sizeof(char **));
    if (!screen->buffer)
    {
        fprintf(stderr, "Error: could not allocate memory for screen buffer\n");
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < screen->width; ++i)
    {
        screen->buffer[i] = (char **)malloc(screen->height * sizeof(char *));
        for (uint32_t j = 0; j < screen->height; ++j)
        {
            screen->buffer[i][j] = (char *)malloc(3 * sizeof(char));
            if (!screen->buffer[i][j])
            {
                fprintf(stderr, "Error: could not allocate memory for screen buffer\n");
                exit(EXIT_FAILURE);
            }
            screen->buffer[i][j][0] = EMPTY_CHAR;
            screen->buffer[i][j][1] = CTERMUI_WHITE;
            screen->buffer[i][j][2] = CTERMUI_BLUE;
        }
    }

    return screen;
}

void ctermui_screen_set_character(ctermui_screen_t s, uint32_t x, uint32_t y, char character, int fg_color, int bg_color)
{
    s->buffer[x][y][0] = character;
    s->buffer[x][y][1] = fg_color;
    s->buffer[x][y][2] = bg_color;
}

void ctermui_screen_free(ctermui_screen_t s)
{
    free(s->buffer);
    free(s);
}

void ctermui_screen_clear(ctermui_screen_t s)
{
    for (uint32_t x = 0; x < s->width; ++x)
    {
        for (uint32_t y = 0; y < s->height; ++y)
        {
            s->buffer[x][y][0] = EMPTY_CHAR;
            s->buffer[x][y][1] = CTERMUI_WHITE;
            s->buffer[x][y][2] = CTERMUI_EMPTY;
        }
    }
}

void ctermui_screen_display(ctermui_screen_t s)
{
    for (uint32_t y = 0; y < s->height; ++y)
    {
        for (uint32_t x = 0; x < s->width; ++x)
        {
            printf("\033[48;5;%dm\033[38;5;%dm%c\033[0m", s->buffer[x][y][2], s->buffer[x][y][1], s->buffer[x][y][0]);
        }
        printf("\n");
    }
}

void sigint_handler(int sig)
{
    __restore_term();
    exit(0);
}
void ctermui_screen_set_widget_root(ctermui_screen_t s, ctermui_widget root)
{
    s->root = root;
}
void ctermui_screen_draw_component(ctermui_screen_t s, ctermui_component c)
{
    if (c->type == TEXT)
    {
        ctermui_screen_draw_component_text(s, c);
    }
    else if (c->type == BUTTON)
    {
        // ctermui_screen_draw_component_button(s, c);
    }
    else
    {
        fprintf(stderr, "ctermui_screen_draw_component: invalid component type\n");
        exit(EXIT_FAILURE);
    }
}

void reload_screen_with_widgets(ctermui_screen_t s, ctermui_widget w)
{
    if (w->type == LEAF && w->component != NULL)
    {
        ctermui_screen_draw_component(s, w->component);
    }
    else
    {
        for (int i = 0; i < w->cc; i++)
        {
            reload_screen_with_widgets(s, w->children[i]);
        }
    }
}

void ctermui_start(ctermui_screen_t s)
{
    signal(SIGINT, sigint_handler);
    while(1){
        __ctermui_screen_clean_term(s);
        //traverse tree s->root and draw components
        ctermui_screen_clear(s);
        reload_screen_with_widgets(s, s->root);
        ctermui_screen_display(s);
        sleep(1);
    }
}

// DRAWING FUNCTIONS

int ctermui_screen_draw_char(ctermui_screen_t s, int x, int y, char c, int fg_color, int bg_color)
{
    if (x < 0 || x >= s->width || y < 0 || y >= s->height)
    {
        fprintf(stderr, "ctermui_screen_draw_char: x=%d, y=%d out of bounds knowing that width=%d and height=%d\n", x, y, s->width, s->height);
        exit(EXIT_FAILURE);
    }
    s->buffer[x][y][0] = c;
    s->buffer[x][y][1] = fg_color;
    s->buffer[x][y][2] = bg_color;
    return 0;
}

int ctermui_screen_draw_line(ctermui_screen_t s, uint16_t orientaion, int x, int y, int length, char c, int color, int bg_color){
    if (orientaion == CTERMUI_HORIZONTAL)
    {
        for (int i = 0; i < length; i++)
        {
            ctermui_screen_draw_char(s, x + i, y, c, color, bg_color);
        }
    }
    else if (orientaion == CTERMUI_VERTICAL)
    {
        for (int i = 0; i < length; i++)
        {
            ctermui_screen_draw_char(s, x, y + i, c, color, bg_color);
        }
    }
    else
    {
        fprintf(stderr, "ctermui_screen_draw_line: invalid orientation\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int ctermui_screen_draw_rect(ctermui_screen_t s, int x, int y, int width, int height, int color, int bg_color)
{
    if (x < 0 || x >= s->width || y < 0 || y >= s->height)
    {
        fprintf(stderr, "ctermui_screen_draw_rect: x or y out of bounds\n");
        exit(EXIT_FAILURE);
    }
    if (x + width >= s->width || y + height >= s->height)
    {
        fprintf(stderr, "ctermui_screen_draw_rect: width or height out of bounds\n");
        exit(EXIT_FAILURE);
    }
    ctermui_screen_draw_char(s, x, y, CTERMUI_TOP_LEFT_CORNER, color, bg_color);
    ctermui_screen_draw_char(s, x + width, y, CTERMUI_TOP_RIGHT_CORNER, color, bg_color);
    ctermui_screen_draw_char(s, x, y + height, CTERMUI_BOTTOM_LEFT_CORNER, color, bg_color);
    ctermui_screen_draw_char(s, x + width, y + height, CTERMUI_BOTTOM_RIGHT_CORNER, color, bg_color);
    ctermui_screen_draw_line(s, CTERMUI_HORIZONTAL, x + 1, y, width - 1, CTERMUI_HORIZONTAL_LINE, color, bg_color);
    ctermui_screen_draw_line(s, CTERMUI_HORIZONTAL, x + 1, y + height, width - 1, CTERMUI_HORIZONTAL_LINE, color, bg_color);
    ctermui_screen_draw_line(s, CTERMUI_VERTICAL, x, y + 1, height - 1, CTERMUI_VERTICAL_LINE, color, bg_color);
    ctermui_screen_draw_line(s, CTERMUI_VERTICAL, x + width, y + 1, height - 1, CTERMUI_VERTICAL_LINE, color, bg_color);
    return 0;
}


int __ctermui_screen_draw_text(ctermui_screen_t s, int x, int y, char* text, int color, int bg_color){
    int i = 0;
    while (text[i] != '\0')
    {
        ctermui_screen_draw_char(s, x + i, y, text[i], color, bg_color);
        i++;
    }
    return 0;
}

int ctermui_screen_draw_component_text(ctermui_screen_t s,  ctermui_component c)
{
    if (c->type != TEXT)
    {
        fprintf(stderr, "ctermui_screen_draw_component_text: invalid component type\n");
        exit(EXIT_FAILURE);
    }

    Text *text = (Text *)c->core_component;
    __ctermui_screen_draw_text(s, c->x, c->y, text->text, text->color, text->bg_color);
    return 0;
}

void ctermui_screen_test(ctermui_screen_t s)
{
    ctermui_screen_draw_rect(s,0,0,10,10,CTERMUI_WHITE,CTERMUI_BLACK);
    ctermui_screen_draw_rect(s,0,0,10,20,CTERMUI_WHITE,CTERMUI_BLACK);
}