
#ifndef _GROOVER_MATRIX_H
#define _GROOVER_MATRIX_H

#include "gfx_types.h"
#include <math.h>

struct Matrix {
    uint8_t rows;
    uint8_t cols;
    float* vals;
};

struct Matrix make_matrix(uint8_t rows, uint8_t cols);

void free_matrix(struct Matrix* m);

void print_matrix(struct Matrix* m);

// // m[r][c] = val
// inline void matrix_set(struct Matrix* m, uint8_t r, uint8_t c, float val);

// // return m[r][c]
// inline float matrix_get(struct Matrix* m, uint8_t r, uint8_t c);

/*
 * Perform matrix dot product
 * M1:NxM, M2:AxN = M3:AxM
 *  [[1, 2], [3, 4], [5, 6]] * [[5, 6, 7], [8, 9, 10]] = 170
 */
float matrix_dot(struct Matrix* m1, struct Matrix* m2);

/*
 * Perform matrix multiplication
 * M1:NxM, M2:AxN = M3:MxN
 * [[1, 2], [3, 4], [5, 6]] * [[5, 6, 7], [8, 9, 10]] = [[21, 24, 27], [47, 54, 61], [73, 84, 95]]
 */
struct Matrix matrix_mul(struct Matrix* m1, struct Matrix* m2);

/*
 * Perform Matrix transpose
 * M:MxN = T:NxM
 * [[1, 2], [3, 4], [5, 6]] = [[1, 3, 5], [2, 4, 6]]
 */
struct Matrix matrix_transpose(struct Matrix* m);

/*
 * Apply M1 to sequence M2
 * M1:MxN, sequence is a vector of length M
 */
struct Matrix matrix_apply(struct Matrix* m1, float* seq, uint16_t n_seq, float* res);

// [[1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]]
struct Matrix make_idenity_matrix(uint8_t n);

// [[sx, 0.0, 0.0], [0.0, sy, 0.0], [0.0, 0.0, 1.0]]
struct Matrix scale_xy(float sx, float sy);

// [[1.0, 0.0, dx], [0.0, 1.0, dy], [0.0, 0.0, 1.0]]
struct Matrix translate_xy(float dx, float dy);

// [[cos(radian(angle)), -sin(radian(angle)), 0.0], [sin(radian(angle)), cos(radian(angle)), 0.0], [0.0, 0.0, 1.0]]
struct Matrix rotate_degrees_xy(float angle);

// [[1.0, sx, 0.0], [sy, 1.0, 0.0], [0.0, 0.0, 1.0]]
struct Matrix shear_xy(float sx, float sy);

// [[-1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]]
struct Matrix reflect_x();

// [[1.0, 0.0, 0.0], [0.0, -1.0, 0.0], [0.0, 0.0, 1.0]]
struct Matrix reflect_y();

/*
b1 and b2 are boxes {left, bottom, right, top} which are point pairs (left, bottom) (right, top)
gives the transformation from box1 to box2
*/
struct Matrix matrix_window(struct PointF32 b1[2], struct PointF32 b2[2]);


#endif /* _GROOVER_MATRIX_H */
