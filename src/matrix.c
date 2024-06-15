
#include <math.h>
#include <stdlib.h>
#include "matrix.h"
#include <stdio.h>

static inline float degrees_to_radians(float degrees) {
    return degrees * (M_PI / 180.0);
}

struct Matrix make_matrix(uint8_t rows, uint8_t cols) {
    float* buff = (float*)malloc(rows*cols*sizeof(float));
    for(uint8_t i=0; i < rows; i++) {
        for(uint8_t j=0; j < cols; j++) {
            buff[i+j] = 0.0;
        }
    }
    return (struct Matrix) { .rows = rows, .cols = cols, .vals = buff };
}

static inline uint8_t matrix_check_bounds(struct Matrix* m, uint8_t r, uint8_t c) {
    if((r > m->rows - 1) || (c > m->cols - 1)) {
        return 1;
    }
    return 0;
}


static inline float matrix_get(struct Matrix* m, uint8_t r, uint8_t c) {
    if(matrix_check_bounds(m, r, c)) {
        fprintf(stderr, "Matrix row, col not in bounds expected within %d x %d, got %d x %d\n", m->rows, m->cols, r, c);
        exit(1);
    }
    uint16_t loc = (r * m->cols) + c;
    return m->vals[loc];
}

static inline void matrix_set(struct Matrix* m, uint8_t r, uint8_t c, float val) {
    if(matrix_check_bounds(m, r, c)) {
        return;
    }
    uint16_t loc = (r * m->cols) + c;
    m->vals[loc] = val;
}

struct Matrix make_idenity_matrix(uint8_t n) {
    struct Matrix m = make_matrix(n, n);
    for(uint8_t i=0; i < n; i++) {
        matrix_set(&m, i, i, 1.0);
    }
    return m;
}

void free_matrix(struct Matrix* m) {
    m->cols = 0;
    m->rows = 0;
    free(m->vals);
}

void print_matrix(struct Matrix* m) {
    printf("[");
    for(int i=0; i < m->rows; i++) {
        printf("\n");
        for(int j=0; j < m->cols; j++) {
            printf("%.5f ", matrix_get(m, i, j));
        }
    }
    printf("\n");
    printf("]\n");
}

float matrix_dot(struct Matrix* m1, struct Matrix* m2) {
    float dot_prod = 0.0;
    for (uint8_t i = 0; i < m1->rows; i++) {
        for (uint8_t j = 0; j < m2->cols; j++) {
            dot_prod += matrix_get(m1, i, j) * matrix_get(m2, i, j);
        }
    }
    return dot_prod;
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

struct Matrix matrix_transpose(struct Matrix* m) {
    struct Matrix t = make_matrix(m->cols, m->rows);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            matrix_set(&t, j, i, matrix_get(m, i, j));
        }
    }
    return t;
}

struct Matrix matrix_apply(struct Matrix* m1, float* seq, uint16_t n_seq, float* res) {
    if(m1->rows != n_seq) {
        fprintf(stderr, "Vector is not same size as matrix rows expected %d got %d\n", m1->rows, n_seq);
        exit(1);
    }
    struct Matrix tmp1 = make_matrix(1, m1->rows);
    struct Matrix tmp2 = matrix_transpose(&tmp1);
    for(uint16_t i = 0; i < n_seq; i++) {
        matrix_set(&tmp2, i, 0, seq[i]);
    }
    struct Matrix tmp3 = matrix_mul(m1, &tmp2);

    for(uint8_t i=0; i < tmp3.rows; i++) {
        res[i] = matrix_get(&tmp3, i, 0);
    }

    free_matrix(&tmp1);
    free_matrix(&tmp2);
    free_matrix(&tmp3);
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

struct Matrix matrix_window(struct PointF32 b1[2], struct PointF32 b2[2]) {
    float scale_x = fabsf(b2[1].x - b2[0].x) / fabsf(b1[1].x - b1[0].x);
    float scale_y = fabsf(b2[1].y - b2[0].y) / fabsf(b1[1].y - b1[0].y);

    struct Matrix tmp0 = translate_xy(b2[0].x, b2[0].y);
    struct Matrix tmp1 = scale_xy(scale_x, scale_y);

    struct Matrix tmp2 = matrix_mul(&tmp0, &tmp1);

    struct Matrix tmp3 = translate_xy(-b1[0].x, -b1[0].y);

    struct Matrix res  = matrix_mul(&tmp2, &tmp3);

    free_matrix(&tmp0);
    free_matrix(&tmp1);
    free_matrix(&tmp2);
    free_matrix(&tmp3);

    return res;
}