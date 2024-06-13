#include <image.h>

int main() {

    struct Image image = make_image(640, 470);
    image.save(&image, "hello.ppm");
    free_image(&image);


    struct Image img = make_image(0, 0);
    img.load(&img, "hello.ppm");
    for(int i=100; i < 200; i++) {
        for(int j=100; j < 200; j++) {
            img.set_pixel(&img, (struct Point) { .x = i, .y = j}, 0, 0, 0);
        }
    }
    img.save(&img, "hello2.ppm");
    free_image(&img);

    return 0;
}