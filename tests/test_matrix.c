
#include <matrix.h>
#include <ryanmock.h>

static void test_matrix_apply1() {
    struct Matrix m = make_matrix(2, 2);
    matrix_set(&m, 0, 0, 1);
    matrix_set(&m, 0, 1, 2);
    matrix_set(&m, 1, 0, 3);
    matrix_set(&m, 1, 1, 4);

    float seq[] = { -1, 1 };
    float res[2] = {0};
    matrix_apply(&m, &seq[0], 2, &res[0]);

    rmmAssertFloatEqual(res[0], 1);
    rmmAssertFloatEqual(res[1], 1);
}

static void test_matrix_apply2() {
    struct Matrix m = make_matrix(2, 2);
    matrix_set(&m, 0, 0, -2);
    matrix_set(&m, 0, 1, 3);
    matrix_set(&m, 1, 0, 1);
    matrix_set(&m, 1, 1, -1);

    float seq[] = { -1, 1 };
    float res[2] = {0};
    matrix_apply(&m, &seq[0], 2, &res[0]);

    rmmAssertFloatEqual(res[0], 5);
    rmmAssertFloatEqual(res[1], -2);
}

int main(int argc, char* argv[]) {
    struct ryanmock_test tests[] = {
        rmmMakeTest(test_matrix_apply1),
        rmmMakeTest(test_matrix_apply2),
    };
    return rmmRunTestsCmdLine(tests, NULL, argc, argv);
}

