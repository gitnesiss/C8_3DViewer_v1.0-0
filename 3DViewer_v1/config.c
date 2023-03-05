#include "config.h"
// int main() {
//     int er = 0;
//     structconfig config;
//     // // ERROR =
//     pars("/Users/jorgedom/C8_3DViewer_v1.0-0/src/object_files/cube.obj",
//     &obj); er =
//     config_load("/Users/tristimu/3DV/src/3DViewer_v1/config_test.ini",
//     &config); if (er == 0) {
//         printf("color_background_r = %d\n", config.color_background_r);
//         printf("color_background_g = %d\n", config.color_background_g);
//         printf("color_background_b = %d\n", config.color_background_b);

//         printf("color_line_r = %d\n", config.color_line_r);
//         printf("color_line_g = %d\n", config.color_line_g);
//         printf("color_line_b = %d\n", config.color_line_b);

//         printf("color_vertex_r = %d\n", config.color_vertex_r);
//         printf("color_vertex_g = %d\n", config.color_vertex_g);
//         printf("color_vertex_b = %d\n", config.color_vertex_b);

//         printf("size_line = %d\n", config.size_line);
//         printf("type_line = %d\n", config.type_line);

//         printf("size_vertex = %d\n", config.size_vertex);
//         printf("type_vertex = %d\n", config.type_vertex);

//         printf("projection = %d\n", config.projection);

//     } else {
//         printf("no FILE!!!\n");
//     }
//     char * paramerts = "234 124 010 115 76 154 76 112 43 1 0 3 1 3";
//     er = config_save("/Users/tristimu/3DV/src/3DViewer_v1/config_test.ini",
//     paramerts); return 0;
// }

void config_save(const char *address, const char *paramerts) {
  FILE *file;
  //    int ERROR = 0;
  file = fopen(address, "w");
  fputs(paramerts, file);
  fclose(file);
  //    return 0;
}

int config_load(const char *address, structconfig *config) {
  FILE *file;
  int ERROR = 0;
  file = fopen(address, "r");
  char stored[800];  // однострочный массив длинной 800
  char *istr;
  if (!file) {
    ERROR = 1;  // файл отсутствует или неоткрылся
  } else {
    fgets(stored, 800, file);
    config->color_background_r = atof(stored);
    istr = strtok(stored, " ");
    istr = strtok(NULL, " ");
    config->color_background_g = atof(istr);
    istr = strtok(NULL, " ");
    config->color_background_b = atof(istr);

    istr = strtok(NULL, " ");
    config->color_line_r = atof(istr);
    istr = strtok(NULL, " ");
    config->color_line_g = atof(istr);
    istr = strtok(NULL, " ");
    config->color_line_b = atof(istr);

    istr = strtok(NULL, " ");
    config->color_vertex_r = atof(istr);
    istr = strtok(NULL, " ");
    config->color_vertex_g = atof(istr);
    istr = strtok(NULL, " ");
    config->color_vertex_b = atof(istr);

    istr = strtok(NULL, " ");
    config->size_line = atoi(istr);
    istr = strtok(NULL, " ");
    config->type_line = atoi(istr);

    istr = strtok(NULL, " ");
    config->size_vertex = atoi(istr);
    istr = strtok(NULL, " ");
    config->type_vertex = atoi(istr);

    istr = strtok(NULL, " ");
    config->projection = atoi(istr);

    fclose(file);
  }
  return ERROR;
}
