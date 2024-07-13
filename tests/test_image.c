
#include <image.h>
#include <ryanmock.h>


static void test_dimensions() {
    struct Image img = make_image(640, 480);
    rmmAssertUIntEqual(img.width, 640);
    rmmAssertUIntEqual(img.height, 480);
    free_image(&img);
}

static void test_get_pixel() {
    struct Image img = make_image(320, 240);
    struct Pixel value = img.get_pixel(&img, (struct Point2I16){200, 200});
    rmmAssertUIntEqual(value.r, 255);
    rmmAssertUIntEqual(value.g, 255);
    rmmAssertUIntEqual(value.b, 255);
    free_image(&img);
}

static void test_set_pixel() {
    struct Image img = make_image(320, 240);
    img.set_pixel(&img, (struct Point2I16){200, 200}, (struct Pixel){255, 0, 0});
    struct Pixel value = img.get_pixel(&img, (struct Point2I16){200, 200});
    rmmAssertUIntEqual(value.r, 255);
    rmmAssertUIntEqual(value.g, 0);
    rmmAssertUIntEqual(value.b, 0);
    free_image(&img);
}

static void test_load_image() {
    struct Image img = make_image(0, 0);
    img.load(&img, "ppm/wartburg.ppm");
    rmmAssertInt16Equal(img.width, 640);
    rmmAssertInt16Equal(img.height, 470);
    struct Pixel value = img.get_pixel(&img, (struct Point2I16){350, 220});
    rmmAssertUIntEqual(value.r, 221);
    rmmAssertUIntEqual(value.g, 156);
    rmmAssertUIntEqual(value.b, 178);
    free_image(&img);
}


int main(int argc, char* argv[]) {
    struct ryanmock_test tests[] = {
        rmmMakeTest(test_dimensions),
        rmmMakeTest(test_get_pixel),
        rmmMakeTest(test_set_pixel),
        rmmMakeTest(test_load_image),
    };
    return rmmRunTestsCmdLine(tests, NULL, argc, argv);
}

