#include "ctermui_screen.h"
#include <unistd.h>

int ctermui_screen_draw_component_button(ctermui_screen_t s, ctermui_component c);
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
    screen->keyboard_events = ctermui_screen_keyboard_events_new();
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

void ctermui_sigint_handler(int sig)
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
        ctermui_screen_draw_component_button(s, c);
    }
    else if(c->type == FRAME)
    {
        ctermui_screen_draw_frame(s,c);
    }else if(c->type == BACKGROUND){
        ctermui_screen_draw_background(s,c);
    }
    else
    {
        fprintf(stderr, "ctermui_screen_draw_component: invalid component type\n");
        exit(EXIT_FAILURE);
    }
}

void reload_screen_with_widgets(ctermui_screen_t s, ctermui_widget w)
{
    int i =0;
    for (i = 0; i < w->component_count; i++)
    {
        ctermui_screen_draw_component(s, w->component[i]);
    }
    for (int i = 0; i < w->cc; i++)
    {
        reload_screen_with_widgets(s, w->children[i]);
    }
}

void ctermui_screen_refresh(ctermui_screen_t s)
{
    __ctermui_screen_clean_term(s);
    ctermui_screen_clear(s);
    ctermui_calculate_abs_position(s->root);
    reload_screen_with_widgets(s, s->root);
    ctermui_screen_display(s);
}

void ctermui_screen_on_resize(ctermui_screen_t* s)
{
    ctermui_screen_free(*s);
    ctermui_widget root = (*s)->root;
    root->absolute_width = __get_term_size().ws_col;
    root->absolute_height = __get_term_size().ws_row;
    *s = ctermui_screen_new();
    ctermui_screen_set_widget_root(*s, root);
    ctermui_screen_refresh(*s);
}

void ctermui_on_resize_listener(ctermui_screen_t* s)
{
    winsize w = __get_term_size();
    if(w.ws_col != (*s)->width || w.ws_row != (*s)->height){
        ctermui_screen_on_resize(s);
    }
}

void set_nonblocking() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_cc[VMIN] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}


void ctermui_on_keybord_listener(ctermui_screen_t* s)
{
    char c;
    if(read(STDIN_FILENO, &c, 1) == 1){
        ctermui_screen_keyboard_events_handle((*s)->keyboard_events, c);
    }
}

void ctermui_start(ctermui_screen_t s)
{
    signal(SIGINT, ctermui_sigint_handler);
    ctermui_screen_refresh(s);
    set_nonblocking();
    while(1){
        ctermui_on_resize_listener(&s);
        ctermui_on_keybord_listener(&s);
        usleep(1000);
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
    if(x + width == s->width){
        width--;
    }
    if(y + height == s->height){
        height--;
    }
    if (x < 0 || x >= s->width || y < 0 || y >= s->height)
    {
        fprintf(stderr, "ctermui_screen_draw_rect: x=%d, y=%d out of bounds knowing that width=%d and height=%d\n", x, y, s->width, s->height);
        exit(EXIT_FAILURE);
    }
    if (x + width >= s->width || y + height >= s->height)
    {
        fprintf(stderr, "ctermui_screen_draw_rect: x=%d, y=%d, width=%d, height=%d out of bounds knowing that width=%d and height=%d\n", x, y, width, height, s->width, s->height);
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
    __ctermui_screen_draw_text(s, c->absolute_x, c->absolute_y, text->text, text->color, text->bg_color);
    return 0;
}

int ctermui_screen_draw_frame(ctermui_screen_t s, ctermui_component c)
{
    if (c->type != FRAME)
    {
        fprintf(stderr, "ctermui_screen_draw_frame: invalid component type\n");
        exit(EXIT_FAILURE);
    }

    Frame *frame = (Frame *)c->core_component;
    ctermui_screen_draw_rect(s, c->absolute_x, c->absolute_y, c->absolute_width, c->absolute_height, frame->color, frame->bg_color);
    return 0;
}

int __ctermui_screen_draw_background(ctermui_screen_t s, int x, int y, int width, int height, int color)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            ctermui_screen_draw_char(s, x + j, y + i, s->buffer[x][y][0], s->buffer[x][y][1], color);
        }
    }
    return 0;
}

int ctermui_screen_draw_background(ctermui_screen_t s, ctermui_component c)
{
    if (c->type != BACKGROUND)
    {
        fprintf(stderr, "ctermui_screen_draw_background: invalid component type\n");
        exit(EXIT_FAILURE);
    }

    Background *background = (Background *)c->core_component;
    for (int y = c->absolute_y; y < c->absolute_y + c->absolute_height; ++y)
    {
        for (int x = c->absolute_x; x < c->absolute_x + c->absolute_width; ++x)
        {
            ctermui_screen_draw_char(s, x, y, s->buffer[x][y][0], s->buffer[x][y][1], background->color);
        }
    }
    
    return 0;
}

int ctermui_screen_draw_component_button(ctermui_screen_t s, ctermui_component c)
{
    if (c->type != BUTTON)
    {
        fprintf(stderr, "ctermui_screen_draw_component_button: invalid component type\n");
        exit(EXIT_FAILURE);
    }

    Button *button = (Button *)c->core_component;
    int text_width = strlen(button->text);
    int frame_width = text_width + 2;
    int frame_height = 3;
    __ctermui_screen_draw_background(s, c->absolute_x, c->absolute_y, frame_width, frame_height, button->bg_color);
    __ctermui_screen_draw_text(s, c->absolute_x + (frame_width - text_width) / 2, c->absolute_y + (frame_height - 1) / 2, button->text, button->text_color, button->bg_color);
    return 0;
}

void ctermui_screen_test(ctermui_screen_t s)
{
    ctermui_screen_draw_rect(s,0,0,10,10,CTERMUI_WHITE,CTERMUI_BLACK);
    ctermui_screen_draw_rect(s,0,0,10,20,CTERMUI_WHITE,CTERMUI_BLACK);
}