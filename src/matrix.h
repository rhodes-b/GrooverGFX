
#ifndef _GROOVER_MATRIX_H
#define _GROOVER_MATRIX_H

#include <gfx_types.h>

struct Matrix {
    uint8_t rows;
    uint8_t cols;
    float* vals;
};

struct Matrix make_matrix(uint8_t rows, uint8_t cols);

void free_matrix(struct Matrix* m);

inline void matrix_set(struct Matrix* m, uint8_t r, uint8_t c, float val);

inline float matrix_get(struct Matrix* m, uint8_t r, uint8_t c);

/*
 * M1:NxM, M2:AxN = M3:MxN
 * [[1, 2], [3, 4], [5, 6]] * [[5, 6, 7], [8, 9, 10]] = [[21, 24, 27], [47, 54, 61], [73, 84, 95]]
 */
struct Matrix matrix_mul(struct Matrix* m1, struct Matrix* m2);

/*
 * M1:NxM, M2:AxN = M3:AxM
 *  [[1, 2], [3, 4], [5, 6]] * [[5, 6, 7], [8, 9, 10]] = 170
 */
struct float matrix_dot(struct Matrix* m1, struct Matrix* m2);

// [[1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]]
struct Matrix make_idenity_matrix(uint8_t size);

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
struct Matrix window(struct PointF32 b1[2], struct PointF32 b2[2]);

#endif /* _GROOVER_MATRIX_H */