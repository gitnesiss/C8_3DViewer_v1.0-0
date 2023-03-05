#ifndef PARS_H
#define PARS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum errors {
  OK,
  err_the_file_did_not_open_or_is_missing,
  err_in_file_not_vertexes,
  err_not_correct_object_file
};

typedef struct modelData {
  int v_count;
  int f_count;
  int lines_count;
  int *arr_vf;  //массив длинною f_count для записи количиства вершин в фасетах
  double **matr_v;
  double *arr_v;
  double *arr_v_copy;
  int *arr_f;
  int **matr_f;
  double min_x;
  double max_x;
  double min_y;
  double max_y;
  double min_z;
  double max_z;
  //    int flag_open_file;
} structobject;

int pars(const char *address, structobject *object);

#endif  // PARS_H
