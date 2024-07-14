
#include <camera.h>
#include <ryanmock.h>

static void test_set_horizontal_view() {
    struct Camera cam = make_camera();

    // FUT
    cam.set_perspective(&cam, 60.0, 1.333, 20);

    rmmAssertFloatEqual(cam.eye.x, 0.00);
    rmmAssertFloatEqual(cam.eye.y, 0.00);
    rmmAssertFloatEqual(cam.eye.z, 0.00);

    rmmAssertFloatEqual(cam.window[0].x, -11.547006);
    rmmAssertFloatEqual(cam.window[0].y, -8.662420);
    rmmAssertFloatEqual(cam.window[1].x, 11.547006);
    rmmAssertFloatEqual(cam.window[1].y, 8.662420);
}

static void test_set_resolution() {
    struct Camera cam = make_camera();
    cam.set_resolution(&cam, 400, 300);

    // FUT
    struct Ray r1 = cam.ij_ray(&cam, -0.5, -0.5);

    rmmAssertFloatEqual(r1.start.x, 0.00);
    rmmAssertFloatEqual(r1.start.y, 0.00);
    rmmAssertFloatEqual(r1.start.z, 0.00);

    rmmAssertFloatEqual(r1.dir.x, -10.00);
    rmmAssertFloatEqual(r1.dir.y, -10.00);
    rmmAssertFloatEqual(r1.dir.z, -10.00);

    // FUT
    struct Ray r2 = cam.ij_ray(&cam, 399.5, 299.5);

    rmmAssertFloatEqual(r2.start.x, 0.00);
    rmmAssertFloatEqual(r2.start.y, 0.00);
    rmmAssertFloatEqual(r2.start.z, 0.00);

    rmmAssertFloatEqual(r2.dir.x, 10.00);
    // TODO: weird rounding error?
    rmmAssertFloatBetween(r2.dir.y, 10.00, 10.000003);
    rmmAssertFloatEqual(r2.dir.z, -10.00);

    // FUT
    struct Ray r3 = cam.ij_ray(&cam, 0., 0.);

    rmmAssertFloatEqual(r3.start.x, 0.00);
    rmmAssertFloatEqual(r3.start.y, 0.00);
    rmmAssertFloatEqual(r3.start.z, 0.00);

    rmmAssertFloatEqual(r3.dir.x, -9.975);
    rmmAssertFloatEqual(r3.dir.y, -9.9666666666);
    rmmAssertFloatEqual(r3.dir.z, -10.00);

    // FUT
    struct Ray r4 = cam.ij_ray(&cam, 399./2, 299./2);
    rmmAssertFloatEqual(r4.start.x, 0.00);
    rmmAssertFloatEqual(r4.start.y, 0.00);
    rmmAssertFloatEqual(r4.start.z, 0.00);

    rmmAssertFloatEqual(r4.dir.x, 0.00);
    // TODO: weird rounding error?
    rmmAssertFloatBetween(r4.dir.y, 0.00, 0.000001);
    rmmAssertFloatEqual(r4.dir.z, -10.00);

}


int main(int argc, char* argv[]) {
    struct ryanmock_test tests[] = {
        rmmMakeTest(test_set_horizontal_view),
        rmmMakeTest(test_set_resolution),
    };
    return rmmRunTestsCmdLine(tests, NULL, argc, argv);
}
