#include "affine_transformations.h"

//void main() {
///* ----------Структура переменных для преобразования из плюсов--------- */
//transformation_values_t transformation_values;
//transformation_values.move_x = 3.;
//transformation_values.move_y = 2.;
//transformation_values.move_z = 1.;
//transformation_values.angle_x = 2.;
//transformation_values.angle_y = 2.;
//transformation_values.angle_z = 2.;
//transformation_values.scale_x = 0.;
//transformation_values.scale_y = 0.;
//transformation_values.scale_z = 0.;
//transformation_values.scale_all = 6.;

///* ----------Структура с матрицей вершин, количеством строк и столбцов*/
///* ----------приходит из плюсов, перезаписывается и возвращается----- */
//matrix_t result;
//result.rows = MAX_SIZE;
//result.columns = 3;
//result.matrix = (double **)calloc(MAX_SIZE, sizeof(double *));
//for (int i = 0; i < MAX_SIZE; i++) {
// result.matrix[i] = (double *)calloc(3, sizeof(double));
//}

//for (int i = 0; i < MAX_SIZE; i++){
//  for (int j = 0; j < 3; j++){
//	result.matrix[i][j] = 1;
//  }
//}

//double time_spent = 0.0;
//clock_t begin = clock();
    
///* ----------Функция аффинных преобразований----- */
//affine_transformations (transformation_values, &result);

//clock_t end = clock();
//time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
//printf("Выполнено за время: %f секунд", time_spent);

//// output(result);
//}

//int affine_transformations (transformation_values_t transformation_values, matrix_t *result) {
//  int error = OK;
//  if (transformation_values.move_x) move_x(result, transformation_values.move_x);
//  if (transformation_values.move_y) move_y(result, transformation_values.move_y);
//  if (transformation_values.move_z) move_z(result, transformation_values.move_z);
//  if (transformation_values.angle_x) rotation_by_ox(result, transformation_values.angle_x);
//  if (transformation_values.angle_y) rotation_by_oy(result, transformation_values.angle_y);
//  if (transformation_values.angle_z) rotation_by_oz(result, transformation_values.angle_z);
//  if (transformation_values.scale_x) scale_x(result, transformation_values.scale_x);
//  if (transformation_values.scale_y) scale_y(result, transformation_values.scale_y);
//  if (transformation_values.scale_z) scale_z(result, transformation_values.scale_z);
//  if (transformation_values.scale_all) scale_all(result, transformation_values.scale_all);
//  return error;
//}

void move_x(structobject *A, double a) {
  for (int i = 0; i < A->v_count; i++) A->matr_v[i][0] += a;
}

void move_y(structobject *A, double a) {
  for (int i = 0; i < A->v_count; i++) A->matr_v[i][1] += a;
}

void move_z(structobject *A, double a) {
  for (int i = 0; i < A->v_count; i++) A->matr_v[i][2] += a;
}

void rotation_by_ox(structobject *A, double angle) {
  double angleToRad = angle * M_PI/180;
  register double val_sin = sin(angleToRad);
  register double val_cos = cos(angleToRad);

  for (int i = 0; i < A->v_count; i++) {
    double temp_y = A->matr_v[i][1];                              //(  1       0        0   )
    double temp_z = A->matr_v[i][2];                              //(  0     cos(a)  -sin(a))
    A->matr_v[i][1] = val_cos * temp_y - val_sin * temp_z;        //(  0     sin(a)   cos(a) )
    A->matr_v[i][2] = val_sin * temp_y + val_cos * temp_z;
  }
}

void rotation_by_oy(structobject *A, double angle) {
  double angleToRad = angle * M_PI/180;
  register double val_sin = sin(angleToRad);
  register double val_cos = cos(angleToRad);

  for (int i = 0; i < A->v_count; i++) {
    double temp_x = A->matr_v[i][0];                              //( cos(a)    0     sin(a))
    double temp_z = A->matr_v[i][2];                              //(   0       1       0   )
    A->matr_v[i][0] = val_cos * temp_x + val_sin * temp_z;        //(-sin(a)    0     cos(a))
    A->matr_v[i][2] = -val_sin * temp_x + val_cos * temp_z;
  }
}

void rotation_by_oz(structobject *A, double angle) {
  double angleToRad = angle * M_PI/180;
  register double val_sin = sin(angleToRad);
  register double val_cos = cos(angleToRad);

  for (int i = 0; i < A->v_count; i++) {
    double temp_x = A->matr_v[i][0];                              //( cos(a)  -sin(a)    0  )
    double temp_y = A->matr_v[i][1];                              //( sin(a)   cos(a)    0  )
    A->matr_v[i][0] = val_cos * temp_x - val_sin * temp_y;  //(   0        0       1  )
    A->matr_v[i][1] = val_sin * temp_x + val_cos * temp_y;
  }
}

//void scale_x(matrix_t *A, double a) {
//  for (int i = 0; i < A->rows; i++) A->matrix[i][0] *= a;
//}

//void scale_y(matrix_t *A, double a) {
//  for (int i = 0; i < A->rows; i++) A->matrix[i][1] *= a;
//}

//void scale_z(matrix_t *A, double a) {
//  for (int i = 0; i < A->rows; i++) A->matrix[i][2] *= a;
//}

void scale_all(structobject *A, double a) {
  for (int i = 0; i < A->v_count; i++) A->matr_v[i][0] *= a;
  for (int i = 0; i < A->v_count; i++) A->matr_v[i][1] *= a;
  for (int i = 0; i < A->v_count; i++) A->matr_v[i][2] *= a;
}


//int output(matrix_t result) {
//  int error = OK;
//  for (int i = 0; i < result.rows; i++){
//  for (int j = 0; j < result.columns; j++){
//    printf("%g ", result.matrix[i][j]);
//  }
//  printf("\n");
//  }
//  return error;
//};
