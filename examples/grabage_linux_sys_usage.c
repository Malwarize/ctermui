#include "ctermui_screen.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NETWORK_USAGE_RANGE 10
#define MAX_CORES 8
int cpu_usage[MAX_CORES + MAX_CORES * 4] = {0};

void generate_data(float xvalues[], float yvalues[]) {
  srand(time(NULL));
  for (int i = 0; i < NETWORK_USAGE_RANGE; ++i) {
    xvalues[i] = i;
    yvalues[i] = 0;
  }
}

void getCPUUsage(int cpu_usage_percentage[]) {
  FILE *file = fopen("/proc/stat", "r");
  if (file == NULL) {
    perror("Error opening /proc/stat");
    exit(EXIT_FAILURE);
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    if (strncmp(line, "cpu", 3) == 0) {
      int core;
      sscanf(line, "cpu%d", &core);

      if (core >= 0 && core < MAX_CORES) {
        int user, nice, sys, idle;
        sscanf(line + 3, "%d %d %d %d", &user, &nice, &sys, &idle);

        if (user < cpu_usage_percentage[core] ||
            nice < cpu_usage_percentage[core + MAX_CORES] ||
            sys < cpu_usage_percentage[core + MAX_CORES * 2] ||
            idle < cpu_usage_percentage[core + MAX_CORES * 3]) {

          cpu_usage_percentage[core] = user;
          cpu_usage_percentage[core + MAX_CORES] = nice;
          cpu_usage_percentage[core + MAX_CORES * 2] = sys;
          cpu_usage_percentage[core + MAX_CORES * 3] = idle;
          continue;
        }

        int total = user + nice + sys + idle;
        int diff_total = total - cpu_usage_percentage[core + MAX_CORES * 4];
        int diff_idle = idle - cpu_usage_percentage[core + MAX_CORES * 3];

        if (diff_total > 0) {
          int usage_percentage = (diff_idle * 100) / diff_total;
          cpu_usage_percentage[core] =
              (usage_percentage > 100) ? 100 : usage_percentage;

          cpu_usage_percentage[core + MAX_CORES * 4] = total;
          cpu_usage_percentage[core + MAX_CORES] = nice;
          cpu_usage_percentage[core + MAX_CORES * 2] = sys;
          cpu_usage_percentage[core + MAX_CORES * 3] = idle;
        }
      }
    }
  }
  fclose(file);
}

void getRamUsage(int *total, int *free) {
  FILE *file = fopen("/proc/meminfo", "r");

  if (file == NULL) {
    perror("Error opening /proc/meminfo");
    exit(EXIT_FAILURE);
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    if (strncmp(line, "MemTotal:", 9) == 0) {
      sscanf(line + 9, "%d", total);
    } else if (strncmp(line, "MemFree:", 8) == 0) {
      sscanf(line + 8, "%d", free);
    }
  }
  fclose(file);
}

void getNetworkUsage(const char *interface, float *incoming, float *outgoing) {
  FILE *file = fopen("/proc/net/dev", "r");

  if (file == NULL) {
    perror("Error opening /proc/net/dev");
    exit(EXIT_FAILURE);
  }

  char buffer[256];
  fgets(buffer, sizeof(buffer), file);
  fgets(buffer, sizeof(buffer), file);

  float in, out;

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    if (sscanf(buffer, "%*s %f %*s %*s %*s %*s %*s %*s %*s %*s %f", &in,
               &out) == 2 &&
        strstr(buffer, interface) != NULL) {

      *incoming = in;
      *outgoing = out;
      break;
    }
  }
  *outgoing /= 1024;
  fclose(file);
}

void periodic(ctermui_screen_t *screen_p) {
  ctermui_screen_t screen = *screen_p;
  ctermui_layout_t root = screen->root;
  ctermui_layout_t w = ctermui_layout_find(root, "wiget_1_1");
  if (w == NULL) {
    fprintf(stderr, "layout not found\n");
  }
  ctermui_component_t c = ctermui_layout_find_component(w, "network");
  ScatterPlot *scatter_plot = (ScatterPlot *)c->core_component;
  int every = 1;
  if (screen->loop_count % every == 0) {

    float incoming, outgoing;
    getNetworkUsage("wlp1s0", &incoming, &outgoing);

    float speed =
        (outgoing - scatter_plot->yvalues[NETWORK_USAGE_RANGE - 1]) / 1024;

    speed = roundf(speed * 1000) / 1000;

    for (int i = 0; i < NETWORK_USAGE_RANGE - 1; i++) {
      scatter_plot->xvalues[i] = scatter_plot->xvalues[i + 1];
      scatter_plot->yvalues[i] = scatter_plot->yvalues[i + 1];
    }
    scatter_plot->xvalues[NETWORK_USAGE_RANGE - 1] =
        scatter_plot->xvalues[NETWORK_USAGE_RANGE - 2] + 1;
    scatter_plot->yvalues[NETWORK_USAGE_RANGE - 1] = speed;
  }
  if (screen->loop_count % 2) {
    ctermui_layout_t cpu_barchart = ctermui_layout_find(root, "wiget_1_2");
    ctermui_component_t barchart =
        ctermui_layout_find_component(cpu_barchart, "barchart");
    BarChart *bar_chart = (BarChart *)barchart->core_component;
    getCPUUsage(cpu_usage);
    ctermui_barchart_update_values(barchart, cpu_usage, MAX_CORES);

    ctermui_layout_t ram_layout = ctermui_layout_find(root, "wiget_2_1_3");
    ctermui_component_t ram_usage =
        ctermui_layout_find_component(ram_layout, "ram_usage");
    ProgressBar *ram_bar = (ProgressBar *)ram_usage->core_component;
    int total, free;
    getRamUsage(&total, &free);
    char usage_string[100];
    sprintf(usage_string, "%dMb/%dGb", (free) / (1024),
            (total) / (1024 * 1024));
    ram_bar->progress = free;
    ram_bar->max = total;
    strcpy(ram_bar->text, usage_string);
  }

  ctermui_screen_refresh_layout(screen, screen->root);
}

int main() {
  ctermui_screen_t screen = ctermui_screen_init();
  ctermui_layout_t root = ctermui_layout_new_root(
      CTERMUI_HORIZONTAL, screen->width, screen->height);

  ctermui_layout_t wiget_1 =
      ctermui_layout_new("left_layout", CTERMUI_VERTICAL, 50);
  ctermui_layout_t wiget_2 =
      ctermui_layout_new("right_layout", CTERMUI_VERTICAL, 50);

  ctermui_layout_t wiget_1_1 =
      ctermui_layout_new("wiget_1_1", CTERMUI_HORIZONTAL, 50);
  ctermui_layout_t wiget_1_2 =
      ctermui_layout_new("wiget_1_2", CTERMUI_HORIZONTAL, 50);

  ctermui_layout_t wiget_2_1 =
      ctermui_layout_new("wiget_2_1", CTERMUI_VERTICAL, 50);

  ctermui_layout_t wiget_2_1_1 = ctermui_layout_new("wiget_2_1_1", LEAF, 25);
  ctermui_layout_t wiget_2_1_2 = ctermui_layout_new("wiget_2_1_2", LEAF, 25);
  ctermui_layout_t wiget_2_1_3 = ctermui_layout_new("wiget_2_1_3", LEAF, 25);
  ctermui_layout_t wiget_2_2 =
      ctermui_layout_new("wiget_2_2", CTERMUI_HORIZONTAL, 50);

  int cpu_usage[MAX_CORES + MAX_CORES * 4] = {0};
  char labels[MAX_CORES][100];

  for (int i = 0; i < MAX_CORES; ++i) {
    cpu_usage[i] = 0;
    sprintf(labels[i], "core %d", i + 1);
  }

  ctermui_component_t barchart =
      ctemrui_new_barchart("barchart", CTERMUI_MAGENTA, CTERMUI_EMPTY, 100,
                           CTERMUI_HORIZONTAL, cpu_usage, labels, MAX_CORES, 1);
  ctermui_layout_add_component(wiget_1_2, barchart);

  float x_values[NETWORK_USAGE_RANGE];
  float y_values[NETWORK_USAGE_RANGE];
  generate_data(x_values, y_values);
  ctermui_component_t linechart = ctermui_new_scatter_plot(
      "network", x_values, y_values, NETWORK_USAGE_RANGE, CTERMUI_CYAN,
      CTERMUI_EMPTY, CTERMUI_RED, 'o', 1);
  ctermui_layout_add_component(wiget_1_1, linechart);

  int total, free;
  getRamUsage(&total, &free);
  char usage_string[100];
  sprintf(usage_string, "%dMb/%dMb", (total - free) / 1024, total / 1024);
  ctermui_component_t ram_usage = ctermui_new_progress_bar(
      "ram_usage", CTERMUI_GREEN, CTERMUI_WHITE, total, free, usage_string,
      CTERMUI_BLACK, CTERMUI_HORIZONTAL);
  ctermui_layout_add_component(wiget_2_1_3, ram_usage);

  ctermui_layout_add_child(wiget_1, wiget_1_1);
  ctermui_layout_add_child(wiget_1, wiget_1_2);

  ctermui_layout_add_child(wiget_2_1, wiget_2_1_1);
  ctermui_layout_add_child(wiget_2_1, wiget_2_1_2);
  ctermui_layout_add_child(wiget_2_1, wiget_2_1_3);
  ctermui_layout_add_child(wiget_2, wiget_2_1);
  ctermui_layout_add_child(wiget_2, wiget_2_2);
  ctermui_layout_add_child(root, wiget_1);
  ctermui_layout_add_child(root, wiget_2);
  ctermui_screen_set_layout_root(screen, root);
  ctermui_screen_loop_start(screen, periodic, 1 * 1000000);
}
