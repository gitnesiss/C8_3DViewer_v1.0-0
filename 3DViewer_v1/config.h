#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct conf {
  int projection;

  int type_line;
  int size_line;

  int type_vertex;
  int size_vertex;

  double color_vertex_r;
  double color_vertex_g;
  double color_vertex_b;

  double color_line_r;
  double color_line_g;
  double color_line_b;

  double color_background_r;
  double color_background_g;
  double color_background_b;
} structconfig;

void config_save(const char* address, const char* paramerts);
int config_load(const char* address, structconfig* config);

#endif  // CONFIG_H
