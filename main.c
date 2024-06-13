#include <image.h>

int main() {

    struct Image image = make_image(640, 470);

    image.save(&image, "./hello.ppm");

    free_image(&image);
    return 0;
}