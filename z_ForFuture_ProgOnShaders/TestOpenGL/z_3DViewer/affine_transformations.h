#ifndef AFFINE_TRANSFORMATIONS_H
#define AFFINE_TRANSFORMATIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "pars.h"

#define MAX_SIZE 1000000

//enum errors {
//  OK,
//  err_not_correct_object_file
//};

//typedef struct transformation_values_struct {
//  double move_x;
//  double move_y;
//  double move_z;
//  double angle_x;
//  double angle_y;
//  double angle_z;
//  double scale_x;
//  double scale_y;
//  double scale_z;
//  double scale_all;
//} transformation_values_t;

//typedef struct matrix_struct {
//  double **matrix;
//  int rows;
//  int columns;
//} matrix_t;

////typedef struct facets {
////  int *vertexes;
////	int numbers_of_vertexes_in_facets;
////} polygon_t;

////typedef struct data {
////    int count_of_vertexes;
////  int count_of_facets;
////  matrix_t matrix_3d;
////  polygon_t *polygons;
////} data_t;


///* --------------------- Функция аффиных преобразований ---------------------*/

//int affine_transformations (transformation_values_t transformation_values, matrix_t *result);

///* ------------------------- Функции сдвига по осям -------------------------*/

/* Сдвиг по Х */
void move_x(structobject *A, double a);

/* Сдвиг по Y */
void move_y(structobject *A, double a);

/* Сдвиг по Z */
void move_z(structobject *A, double a);

/* ------------------- Функции поворота относительно осей -------------------*/

/* Поворот относительно Х */
void rotation_by_ox(structobject *A, double angle);

/* Поворот относительно Y */
void rotation_by_oy(structobject *A, double angle);

/* Поворот относительно Z */
void rotation_by_oz(structobject *A, double angle);

///* -------------------- Функции масштабирования по осям --------------------*/

///* Масштабирование по Х */
//void scale_x(matrix_t *A, double a);

///* Масштабирование по Y */
//void scale_y(matrix_t *A, double a);

///* Масштабирование по Z */
//void scale_z(matrix_t *A, double a);

/* Масштабирование по всем осям */
void scale_all(structobject *A, double a);

///* ------------------------- Вывод матрицы вершин -------------------------*/
//int output(matrix_t result);

#endif  // AFFINE_TRANSFORMATIONS_H
