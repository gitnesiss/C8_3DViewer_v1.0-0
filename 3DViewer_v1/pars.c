#include "pars.h"

#include "affine_transformations.h"

// int main () {
//  int ERROR = OK;
//  structobject obj;
//  ERROR = pars("/Users/jorgedom/C8_3DViewer_v1.0-0/src/object_files/cube.obj",
//  &obj);

//  printf("ERROR = %d\n", ERROR);

// //  printf("object->arr_vf = ");
// //   for (int i = 0; i < obj.f_count; i++) {
// //       printf("%d ", obj.arr_vf[i]);
// //   }

// //  printf("\nmin_x = %g\n", obj.min_x);
// //  printf("max_x = %g\n", obj.max_x);
// //  printf("min_y = %g\n", obj.min_y);
// //  printf("max_y = %g\n", obj.max_y);
// //  printf("min_z = %g\n", obj.min_z);
// //  printf("max_z = %g\n", obj.max_z);
// //  printf("obj.v_count = %d\n", obj.v_count);
// //  printf("obj.f_count = %d\n", obj.f_count);
//  printf("--------------------------\nfaset =:");
//   for (int i = 0; i < obj.f_count; i++) {
//     printf("\nobj.matr_f[%d] = ",i);
//     for (int j = 0; j < obj.arr_vf[i]; j++) {
//         printf("%3d ", obj.matr_f[i][j]);

//     }
//   }

//   for (int i = 0; i < obj.v_count; i++) {
//     printf("\nobj.matr_v[%d] = ", i);
//     for (int j = 0; j < 3; j++) {
//         printf("%x ", &obj.matr_v[i][j]);
//         printf("%g ", obj.matr_v[i][j]);
//     }
//   }

//  return 0;
// }

int pars(const char *address, structobject *object) {
  FILE *file;
  int ERROR = OK;
  object->lines_count = 0;
  file = fopen(address, "r");
  if (!file) {
    ERROR = err_the_file_did_not_open_or_is_missing;  // файл отсутствует или
                                                      // неоткрылся
  } else {
    char stored[800];  // однострочный массив длинной 800
    char *istr;
    object->v_count = 0;  //  1 для того, что при отрисовке вершина берется с 1,
                          //  а не с 0 элемента в массиве
    object->f_count = 0;
    // считаем кол-во вершин и фасетов--------------
    while (!feof(file)) {
      fgets(stored, 800, file);
      // printf("stored[i] = %s", stored[i]);
      if (!strncmp(stored, "v ", 2)) {
        object->v_count += 1;
      } else if (!strncmp(stored, "f ", 2)) {
        // printf("1\n");
        object->f_count += 1;
      }
    }
    if (object->v_count == 0) ERROR = err_in_file_not_vertexes;
    fclose(file);

    // считаем кол-во вершин в фасетах--------------

    if (!ERROR) {
      // -----выделяем память под массив количества вершин в фасетах------
      object->arr_vf = (int *)calloc(object->f_count, sizeof(int));
      file = fopen(address, "r");
      for (int i = 0, j = 0; !feof(file); i++) {
        // printf("старт for\n");
        fgets(stored, 800, file);
        // printf("stored = %s\n", stored);
        if (!strncmp(stored, "f ", 2)) {
          // printf("\nисходная строка = %s\n", stored);
          object->arr_vf[j] =
              0;  // колличество вершин в фасете (-1 чтобы вычесть букву из
                  // количества токенов в строке)
          istr = strtok(stored, "f ");
          // printf("\nistr = %s\n", istr);
          while (istr != NULL && strncmp(istr, "\n", 2) && atoi(istr) > 0) {
            // printf("\nistr до токенизации= %s\n", istr);
            istr = strtok(NULL, " ");
            // printf("\nistr  после токенизации= %s\n", istr);
            object->arr_vf[j] += 1;
          }
          j++;
        }
      }
      fclose(file);
      file = fopen(address, "r");
      //-----выделяем память под матрицу вершин object->matr_v --------
      object->matr_v = (double **)calloc(object->v_count, sizeof(double *));
      for (int i = 0; i < object->v_count; i++) {
        object->matr_v[i] = (double *)calloc(3, sizeof(double));
      }
      // -----выделяем память под массив поверхностей object->matr_f------
      object->matr_f = (int **)calloc(object->f_count, sizeof(int *));
      // матрица будет зубчатая (зубчатый массив)
      for (int i = 0; i < object->f_count; i++) {
        object->matr_f[i] = (int *)calloc(object->arr_vf[i], sizeof(int));
      }
      int matr_f_num = 0;
      for (int i = 0; !feof(file) && ERROR == OK;) {
        fgets(stored, 800, file);
        if (!strncmp(stored, "v ", 2)) {
          //    printf("stored[i] = %s",stored);
          istr = strtok(stored, "v ");
          // printf("istr1 = %s\n", istr);
          object->matr_v[i][0] = atof(istr);
          istr = strtok(NULL, " ");
          object->matr_v[i][1] = atof(istr);
          istr = strtok(NULL, " ");
          object->matr_v[i][2] = atof(istr);
          i++;
        } else if (!strncmp(stored, "f ", 2)) {
          istr = strtok(stored, "f ");
          // printf("istr1 = %s\n", istr);
          object->matr_f[matr_f_num][0] = atoi(istr);
          object->lines_count += 1;
          for (int i = 1; i < object->arr_vf[matr_f_num]; i++) {
            istr = strtok(NULL, " ");
            object->matr_f[matr_f_num][i] = atoi(istr);
            object->lines_count += 1;
          }
          matr_f_num++;
        }
      }
      fclose(file);
      // вычисляем минимальный и максимальные координаты
      object->min_x = object->matr_v[0][0],
      object->max_x = object->matr_v[0][0],
      object->min_y = object->matr_v[0][1],
      object->max_y = object->matr_v[0][1],
      object->min_z = object->matr_v[0][2],
      object->max_z = object->matr_v[0][2];
      for (int i = 0; i < object->v_count; i++) {
        if (object->matr_v[i][0] < object->min_x)
          object->min_x = object->matr_v[i][0];
        if (object->matr_v[i][0] > object->max_x)
          object->max_x = object->matr_v[i][0];
        if (object->matr_v[i][1] < object->min_y)
          object->min_y = object->matr_v[i][1];
        if (object->matr_v[i][1] > object->max_y)
          object->max_y = object->matr_v[i][1];
        if (object->matr_v[i][2] < object->min_z)
          object->min_z = object->matr_v[i][2];
        if (object->matr_v[i][2] > object->max_z)
          object->max_z = object->matr_v[i][2];
      }
      //  выделяем память под массив фасетов
      object->lines_count *= 2;
      object->arr_f = (int *)calloc(object->lines_count, sizeof(int));
      for (int i = 0, k = 0; i < object->f_count; i++) {
        for (int j = 0; j < object->arr_vf[i]; j++) {
          if (j != object->arr_vf[i] - 1) {
            object->arr_f[k] = object->matr_f[i][j] - 1;
            k++;
            object->arr_f[k] = object->matr_f[i][j + 1] - 1;
            k++;
          } else {
            object->arr_f[k] = object->matr_f[i][0] - 1;
            k++;
            object->arr_f[k] = object->matr_f[i][j] - 1;
            k++;
          }
        }
      }
      object->arr_v = (double *)calloc((object->v_count * 3), sizeof(double));
      for (int i = 0, k = 0; i < object->v_count; i++) {
        object->arr_v[k] = object->matr_v[i][0];
        object->arr_v[k + 1] = object->matr_v[i][1];
        object->arr_v[k + 2] = object->matr_v[i][2];
        k += 3;
      }
      object->arr_v_copy =
          (double *)calloc((object->v_count * 3), sizeof(double));
    }
  }
  return ERROR;
}
