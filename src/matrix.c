
#include <math.h>
#include <stdlib.h>
#include "matrix.h"

static inline float degrees_to_radians(float degrees) {
    return degrees * (M_PI / 180.0);
}

struct Matrix make_matrix(uint8_t rows, uint8_t cols) {
    float* buff = (float*)malloc(rows*cols*sizeof(float));
    return (struct Matrix) { .rows = rows, .cols = cols, .vals = buff };
}

struct Matrix make_idenity_matrix(uint8_t size) {
    struct Matrix m = make_matrix(size, size);
    for(uint8_t i=0; i < size; i++) {
        matrix_set(&m, i, i, 1.0);
    }
}

void free_matrix(struct Matrix* m) {
    m->cols = 0;
    m->rows = 0;
    free(m->vals);
}

inline void matrix_set(struct Matrix* m, uint8_t r, uint8_t c, float val) {
    // stored row wise so 3x2 = [[0,1], [2,3], [4,5]]
    uint16_t loc = (r * m->cols) + c;
    if((loc < 0) || (loc > m->cols*m->rows - 1)) {
        return;
    }
    m->vals[loc] = val;
}

inline float matrix_get(struct Matrix* m, uint8_t r, uint8_t c) {
    uint16_t loc = (r * m->cols) + c;
    if((loc < 0) || (loc > m->cols*m->rows - 1)) {
        return 0.0;
    }
    return m->vals[loc];
}

struct Matrix matrix_mul(struct Matrix* m1, struct Matrix* m2) {
    struct Matrix m3 = make_matrix(m1->rows, m2->cols);
    for (uint8_t i = 0; i < m1->rows; i++) {
        for (uint8_t j = 0; j < m2->cols; j++) {
            matrix_set(&m3, i, j, 0);
            for (int k = 0; k < m1->cols; k++) {
                float val = matrix_get(&m3, i, j) + matrix_get(m1, i, k) * matrix_get(m2, k, j);
                matrix_set(&m3, i, j, val);
            }
        }
    }
    return m3;
}

float matrix_dot(struct Matrix* m1, struct Matrix* m2) {
    float dotProduct = 0.0;
    for (uint8_t i = 0; i < m1->rows; i++) {
        for (uint8_t j = 0; j < m2->cols; j++) {
            dotProduct += matrix_get(&m1, i, j) * matrix_get(&m2, i, j);
        }
    }
    return dotProduct;
}

struct Matrix scale_xy(float sx, float sy) {
    struct Matrix m = make_matrix(3, 3);
    matrix_set(&m, 0, 0, sx);
    matrix_set(&m, 1, 1, sy);
    matrix_set(&m, 2, 2, 1.0);
    return m;
}

struct Matrix translate_xy(float dx, float dy) {
    struct Matrix m = make_idenity_matrix(3);
    matrix_set(&m, 0, 2, dx);
    matrix_set(&m, 1, 2, dy);
    return m;
}

struct Matrix rotate_degrees_xy(float angle) {
    struct Matrix m = make_matrix(3, 3);
    float rad = degrees_to_radians(angle);
    matrix_set(&m, 0, 0, (float)cos(rad));
    matrix_set(&m, 0, 1, (float)-sin(rad));
    matrix_set(&m, 1, 0, (float)sin(rad));
    matrix_set(&m, 1, 1, (float)cos(rad));
    matrix_set(&m, 2, 2, 1.0);
    return m;
}

struct Matrix shear_xy(float sx, float sy) {
    struct Matrix m = make_matrix(3, 3);
    float rad_x = degrees_to_radians(sx);
    float rad_y = degrees_to_radians(sy);
    matrix_set(&m, 0, 0, 1.0);
    matrix_set(&m, 0, 1, (float)tan(degrees_to_radians(sx)));
    matrix_set(&m, 1, 0, (float)tan(degrees_to_radians(sy)));
    matrix_set(&m, 1, 1, 1.0);
    matrix_set(&m, 2, 2, 1.0);
    return m;
}

struct Matrix reflect_x() {
    struct Matrix m = make_idenity_matrix(3);
    matrix_set(&m, 0, 0, -1.0);
    return m;
}

struct Matrix reflect_y() {
    struct Matrix m = make_idenity_matrix(3);
    matrix_set(&m, 1, 1, -1.0);
    return m;
}

struct Matrix window(struct PointF32 b1[2], struct PointF32 b2[2]) {
    /*
    scale_x = abs(box1[2] - box1[0]) / abs(box0[2] - box0[0])
    scale_y = abs(box1[3] - box1[1]) / abs(box0[3] - box0[1])
    return 
        x = mat.mul(translate(box1[0], box1[1]), scale(scale_x, scale_y)),
        return mat.mul(x, translate(-box0[0], -box0[1]))
    */
    float scale_x = fabsf(b2[1].x - b2[0].x) / fabsf(b1[1].x - b1[0].x);
    float scale_y = fabsf(b2[1].y - b2[0].y) / fabsf(b1[1].y - b1[0].y);

    struct Matrix tmp0 = translate_xy(b2[0].x, b2[0].y);
    struct Matrix tmp1 = scale_xy(scale_x, scale_y);

    struct Matrix tmp2 = matrix_mul(&tmp0, &tmp1);

    struct Matrix tmp3 = translate_xy(-b1[0].x, -b2[0].y);
    struct Matrix res  = matrix_mul(&tmp2, &tmp3);

    matrix_free(tmp0);
    matrix_free(tmp1);
    matrix_free(tmp2);
    matrix_free(tmp3);

    return res;
}