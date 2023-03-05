#include "affine_transformations.h"

void all_rotation(structobject *A, double mov_x, double mov_y, double mov_z,
                  double angle_x, double angle_y, double angle_z,
                  double scale) {
  // printf (" mov_x = %g  mov_y = %g  mov_z = %g     angle_x = %g angle_y = %g
  // angle_z = %g    scale = %g\n\n", mov_x, mov_y, mov_z, angle_x, angle_y,
  // angle_z, scale);
  for (int i = 0; i < A->v_count * 3; i += 3)
    A->arr_v_copy[i] = A->arr_v[i] + mov_x;
  for (int i = 1; i < A->v_count * 3; i += 3)
    A->arr_v_copy[i] = A->arr_v[i] + mov_y;
  for (int i = 2; i < A->v_count * 3; i += 3)
    A->arr_v_copy[i] = A->arr_v[i] + mov_z;

  double angleToRad = angle_x * M_PI / 180;
  register double val_sin = sin(angleToRad);
  register double val_cos = cos(angleToRad);

  for (int i = 0; i < A->v_count * 3; i += 3) {
    double temp_y = A->arr_v_copy[i + 1];  //(  1       0        0   )
    double temp_z = A->arr_v_copy[i + 2];  //(  0     cos(a)  -sin(a))
    A->arr_v_copy[i + 1] =
        val_cos * temp_y - val_sin * temp_z;  //(  0     sin(a)   cos(a) )
    A->arr_v_copy[i + 2] = val_sin * temp_y + val_cos * temp_z;
  }

  angleToRad = angle_y * M_PI / 180;
  val_sin = sin(angleToRad);
  val_cos = cos(angleToRad);

  for (int i = 0; i < A->v_count * 3; i += 3) {
    double temp_x = A->arr_v_copy[i];      //( cos(a)    0     sin(a))
    double temp_z = A->arr_v_copy[i + 2];  //(   0       1       0   )
    A->arr_v_copy[i] =
        val_cos * temp_x + val_sin * temp_z;  //(-sin(a)    0     cos(a))
    A->arr_v_copy[i + 2] = -val_sin * temp_x + val_cos * temp_z;
  }

  angleToRad = angle_z * M_PI / 180;
  val_sin = sin(angleToRad);
  val_cos = cos(angleToRad);

  for (int i = 0; i < A->v_count * 3; i += 3) {
    double temp_x = A->arr_v_copy[i];      //( cos(a)  -sin(a)    0  )
    double temp_y = A->arr_v_copy[i + 1];  //( sin(a)   cos(a)    0  )
    A->arr_v_copy[i] =
        val_cos * temp_x - val_sin * temp_y;  //(   0        0       1  )
    A->arr_v_copy[i + 1] = val_sin * temp_x + val_cos * temp_y;
  }

  for (int i = 0; i < A->v_count * 3; i++)
    A->arr_v_copy[i] = A->arr_v_copy[i] * scale;
}

void free_struct(structobject *A) {
  free(A->arr_v_copy);
  free(A->arr_f);
  free(A->arr_vf);
  free(A->arr_v);
  for (int i = 0; i < A->v_count; i++) {
    free(A->matr_v[i]);
  }
  free(A->matr_v);
  for (int i = 0; i < A->f_count; i++) {
    free(A->matr_f[i]);
  }
  free(A->matr_f);
}
