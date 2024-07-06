
#include <image.h>
#include <ryanmock.h>

/*

r"""Simple raster image. Allows pixel-level access and saving
                and loading as PPM image files.

                Examples:
                >>> img = Image((320, 240))    # create a 320x240 image
                >>> img.size
                (320, 240)
                >>> img[200,200]  # get color at pixel (200,200)
                (0, 0, 0)
                >>> img[200, 100] = (255, 0, 0) # set pixel to bright red
                >>> img[200, 100]   # get color of the pixel back again
                (255, 0, 0)
                >>> img.save("ppm/reddot.ppm")    # save image to a ppm file
                >>> img = Image((2, 3))
                >>> img[0,0] = 148, 103, 82
                >>> img[1,2] = 13, 127, 255
                >>> img.getdata()  # dump image data in ppm format
                b'P6\n2 3\n255\n\x00\x00\x00\r\x7f\xff\x00\x00\x00\x00\x00\x00\x94gR\x00\x00\x00'
                >>> img.load("ppm/wartburg.ppm")  # load a ppm image
                >>> img.size
                (640, 470)
                >>> img[350, 220]
                (148, 103, 82)
                >>> img.clear((255,255,255))  # make image all white
                >>> img.save("ppm/blank.ppm")     # blank.ppm is 640x470 all white
                """

*/

static void test_dimensions() {
    struct Image img = make_image(640, 480);
    rmmAssertInt16Equal(img.width, 640);
    rmmAssertInt16Equal(img.height, 480);
}

int main(int argc, char* argv[]) {
    struct ryanmock_test tests[] = {
        rmmMakeTest(test_dimensions),
    };

    return rmmRunTestsCmdLine(tests, NULL, argc, argv);

}

