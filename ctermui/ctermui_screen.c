#include "ctermui_screen.h"


winsize __get_term_size()
{
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w;
}

const char EMPTY_CHAR = ' ';

void ctermui_screen_clean_term()
{
    printf("\033[?25l");
    system("clear");
}

void ctermui_restore_cursor()
{
    printf("\033[?25h");
}


void allocate_screen_buffer(ctermui_screen_t s)
{
    s->buffer = (char ***)malloc(s->width * sizeof(char **));
    if (!s->buffer)
    {
        fprintf(stderr, "Error: could not allocate memory for screen buffer\n");
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < s->width; i++)
    {
        s->buffer[i] = (char **)malloc(s->height * sizeof(char *));
        if (!s->buffer[i])
        {
            fprintf(stderr, "Error: could not allocate memory for screen buffer\n");
            exit(EXIT_FAILURE);
        }
        for (uint32_t j = 0; j < s->height; j++)
        {
            s->buffer[i][j] = (char *)malloc(3 * sizeof(char));
            if (!s->buffer[i][j])
            {
                fprintf(stderr, "Error: could not allocate memory for screen buffer\n");
                exit(EXIT_FAILURE);
            }
            s->buffer[i][j][0] = EMPTY_CHAR;
            s->buffer[i][j][1] = CTERMUI_WHITE;
            s->buffer[i][j][2] = CTERMUI_BLUE;
        }
    }
    if(!s->buffer)
    {
        fprintf(stderr, "Error: could not allocate memory for screen buffer\n");
        exit(EXIT_FAILURE);
    }
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
    // loop state
    screen->loop_count = 0;
    screen->loop_running = 0;
    screen->loop_stop = 0;
    screen->loop_idle = 0;
    allocate_screen_buffer(screen);
    return screen;
}

void ctermui_screen_set_character(ctermui_screen_t s, uint32_t x, uint32_t y, char character, int fg_color, int bg_color)
{
    s->buffer[x][y][0] = character;
    s->buffer[x][y][1] = fg_color;
    s->buffer[x][y][2] = bg_color;
}
int ctermui_screen_free_buffer(ctermui_screen_t s) {
    for(size_t i = 0; i <  s->width; i++)
    {
        for(size_t j = 0; j <s->height; j++)
        {
            free(s->buffer[i][j]);
        }
        free(s->buffer[i]);
    }
    free(s->buffer);
    s->buffer = NULL;
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

void ctermui_screen_clear_part(ctermui_screen_t s, int x, int y, int width, int height)
{
    for (int i = y; i < y+height; i++)
    {
        for (int j = x; j < x+width; j++)
        {
            s->buffer[j][i][0] = EMPTY_CHAR;
            s->buffer[j][i][1] = CTERMUI_WHITE;
            s->buffer[j][i][2] = CTERMUI_EMPTY;
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
    }
}


void clean_up(ctermui_screen_t s){
    // fprintf(stderr,"cleaning up\n");
    // fprintf(stderr,"freeing buffer\n");
    // ctermui_screen_free_buffer(s);
    // ctermui_screen_keyboard_events_free(s->keyboard_events);
    // free(s);
}

void ctermui_sigint_handler(int sig, void *data)
{
    ctermui_screen_t s = (ctermui_screen_t)data;
    clean_up(s);
    ctermui_restore_cursor();
    exit(0);
}

void ctermui_screen_set_widget_root(ctermui_screen_t s, ctermui_widget root)
{
    s->root = root;
}

void ctermui_screen_draw_component(ctermui_screen_t s, ctermui_component c)
{
    c->draw(s, c);
}

void ctermui_screen_draw_all_components_of_widget(ctermui_screen_t s, ctermui_widget w)
{
    int i = 0;
    for (i = 0; i < w->component_count; i++)
    {
        if(w->component[i] && w->component[i]->draw == NULL){
            fprintf(stderr,"%s:%d: Error: component %s has no draw function\n",__FILE__,__LINE__,w->component[i]->id);
            exit(EXIT_FAILURE);
        }
        w->component[i]->draw(s, w->component[i]);
    }
    for (int i = 0; i < w->children_count; i++)
    {
        ctermui_screen_draw_all_components_of_widget(s, w->children[i]);
    }
}

void ctermui_screen_redraw_all_components_of_widget(ctermui_screen_t s, ctermui_widget new_w, int old_x, int old_y, int old_width, int old_height)
{
    ctermui_screen_clear_part(s, old_x, old_y, old_width, old_height);
    ctermui_screen_draw_all_components_of_widget(s, new_w);
}

void ctermui_screen_refresh_widget(ctermui_screen_t s, ctermui_widget w)
{
    int old_x = w->absolute_x;
    int old_y = w->absolute_y;
    int old_width = w->absolute_width;
    int old_height = w->absolute_height;
    ctermui_calculate_abs_position(s->root);
    ctermui_screen_redraw_all_components_of_widget(s, w, old_x, old_y, old_width, old_height);
    ctermui_screen_display_widget(s, w);
}

void ctermui_screen_refresh_widgets(ctermui_screen_t s)
{
    ctermui_screen_clean_term();
    ctermui_screen_clear(s);
    ctermui_calculate_abs_position(s->root);
    ctermui_screen_draw_all_components_of_widget(s, s->root);
    ctermui_screen_display(s);
}

void ctermui_screen_on_resize(ctermui_screen_t* sp)
{
    winsize w = __get_term_size();
    int width = w.ws_col;
    int height = w.ws_row;
    ctermui_screen_free_buffer(*sp);
    (*sp)->width = width;
    (*sp)->height = height;
    (*sp)->root->absolute_width = width;
    (*sp)->root->absolute_height = height;
    allocate_screen_buffer(*sp);
    ctermui_screen_refresh_widgets(*sp);
}

int ctermui_on_resize_listener(ctermui_screen_t *s)
{
    winsize w = __get_term_size();
    if (w.ws_col != (*s)->width || w.ws_row != (*s)->height)
    {
        ctermui_screen_on_resize(s);
        return 1;
    }
    else
    {
        return 0;
    }
}

int ctermui_kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void ctermui_on_keybord_listener(ctermui_screen_t *s)
{
    char c;
    if (ctermui_kbhit())
    {
        c = getchar();
        // arrows have 3 chars
        if(c == '\033')
        {
            //skip [
            getchar();
            c = getchar();
            switch(c)
            {
                case 'A':
                    c = CTERMUI_KEY_UP;
                    break;
                case 'B':
                    c = CTERMUI_KEY_DOWN;
                    break;
                case 'C':
                    c = CTERMUI_KEY_RIGHT;
                    break;
                case 'D':
                    c = CTERMUI_KEY_LEFT;
                    break;
                case 'H':
                    c = CTERMUI_KEY_ESC;
                    break;
                case 'F':
                    c = CTERMUI_KEY_ENTER;
                    break;
                case 'Z':
                    c = CTERMUI_KEY_TAB;
                    break;                 
            }
        }
        ctermui_screen_keyboard_events_handle((*s)->keyboard_events, c);
    }
}

void ctermui_screen_display_part(ctermui_screen_t s, int x, int y, int width, int height){
    // adjust the cursor position
    printf("\033[%d;%dH", y+1, x+1);
    for (int i = y; i < y+height; i++)
    {
        for (int j = x; j < x+width; j++)
        {
            printf("\033[48;5;%dm\033[38;5;%dm%c\033[0m", s->buffer[j][i][2], s->buffer[j][i][1], s->buffer[j][i][0]);   
        }
    }
}

void ctermui_screen_display_widget(ctermui_screen_t s, ctermui_widget w){
    ctermui_screen_display_part(s, w->absolute_x, w->absolute_y, w->absolute_width, w->absolute_height);
}

void ctermui_screen_loop_shutdown(ctermui_screen_t s)
{
    s->loop_stop = 1;
}

void ctermui_screen_loop_pause(ctermui_screen_t s)
{
    s->loop_idle = 1;
}

void ctermui_screen_loop_resume(ctermui_screen_t s)
{
    s->loop_idle = 0;
}


struct termios orig_termios;

void ctermui_disable_raw_mode() {
    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void exit_trigger(void){
    ctermui_disable_raw_mode();
}
void ctermui_enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(ctermui_disable_raw_mode); // Ensure disableRawMode is called at exit

    struct termios raw = orig_termios;

    // Modify the settings for raw mode
    raw.c_lflag &= ~(ECHO | ICANON);

    // Apply the modified settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
void ctermui_screen_loop_start(ctermui_screen_t s, void (*periodic_func)(ctermui_screen_t*), int every)
{
    ctermui_enable_raw_mode();
    signal(SIGINT,(void (*)(int))ctermui_sigint_handler);
    ctermui_screen_refresh_widgets(s);
    while (1)
    {
        if (s->loop_idle)
        {
            usleep(every);
            continue;
        }

        if(ctermui_on_resize_listener(&s))
            continue;

        if(periodic_func)
            periodic_func(&s);
        ctermui_on_keybord_listener(&s);
        if (s->loop_stop)
        {
            break;
        }
        usleep(every);
        s->loop_count++;
    }
}

