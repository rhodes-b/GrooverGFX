
#include <ray3d.h>
#include <ryanmock.h>


static void test_ray_init() {
    // FUT
    struct Ray r1 = make_ray((struct Point3F32){0., 1., 2.}, (struct Vec3){1., 2., 1.});
    rmmAssertFloatEqual(r1.start.x, 0.00);
    rmmAssertFloatEqual(r1.start.y, 1.00);
    rmmAssertFloatEqual(r1.start.z, 2.00);

    rmmAssertFloatEqual(r1.dir.x, 1.00);
    rmmAssertFloatEqual(r1.dir.y, 2.00);
    rmmAssertFloatEqual(r1.dir.z, 1.00);

    // FUT
    struct Ray r2 = make_ray((struct Point3F32){1., 2., 3.}, (struct Vec3){4., 5., 6.});
    rmmAssertFloatEqual(r2.start.x, 1.00);
    rmmAssertFloatEqual(r2.start.y, 2.00);
    rmmAssertFloatEqual(r2.start.z, 3.00);

    rmmAssertFloatEqual(r2.dir.x, 4.00);
    rmmAssertFloatEqual(r2.dir.y, 5.00);
    rmmAssertFloatEqual(r2.dir.z, 6.00);

}

static void test_ray_at() {
    struct Ray r = make_ray((struct Point3F32){0., 1., 2.}, (struct Vec3){1., 2., 3.});

    // FUT
    struct Point3F32 p1 = r.point_at(&r, 0.);
    rmmAssertFloatEqual(p1.x, 0.00);
    rmmAssertFloatEqual(p1.y, 1.00);
    rmmAssertFloatEqual(p1.z, 2.00);

    // FUT
    struct Point3F32 p2 = r.point_at(&r, 1.);
    rmmAssertFloatEqual(p2.x, 1.00);
    rmmAssertFloatEqual(p2.y, 3.00);
    rmmAssertFloatEqual(p2.z, 5.00);

    // FUT
    struct Point3F32 p3 = r.point_at(&r, 3.75);
    rmmAssertFloatEqual(p3.x, 3.75);
    rmmAssertFloatEqual(p3.y, 8.50);
    rmmAssertFloatEqual(p3.z, 13.25);
}


int main(int argc, char* argv[]) {
    struct ryanmock_test tests[] = {
        rmmMakeTest(test_ray_init),
        rmmMakeTest(test_ray_at),
    };
    return rmmRunTestsCmdLine(tests, NULL, argc, argv);
}