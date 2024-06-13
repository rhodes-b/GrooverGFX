#include <image.h>

int main() {

    struct Image image = make_image(640, 470);

    // left edge has red and right has blue?
    // not sure why
    for(int i=100; i < 200; i++) {
        for(int j=100; j < 200; j++) {
            image.set_pixel(&image, (struct Point) { .x = i, .y = j}, 0, 0, 0);
        }
    }
    
    image.save(&image, "./hello.ppm");

    free_image(&image);
    return 0;
}