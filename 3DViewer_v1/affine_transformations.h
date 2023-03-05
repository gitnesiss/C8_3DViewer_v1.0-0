#ifndef AFFINE_TRANSFORMATIONS_H
#define AFFINE_TRANSFORMATIONS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "pars.h"

#define MAX_SIZE 1000000

void all_rotation(structobject *A, double mov_x, double mov_y, double mov_z,
                  double angle_x, double angle_y, double angle_z, double scale);

void free_struct(structobject *A);

#endif  // AFFINE_TRANSFORMATIONS_H
