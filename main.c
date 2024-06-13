#include <image.h>
#include <painter.h>

int main() {

    struct Image image = make_image(640, 470);
    image.save(&image, "ppm/hello.ppm");
    free_image(&image);

    struct Image img = make_image(0, 0);
    img.load(&img, "ppm/hello.ppm");
    for(int i=100; i < 200; i++) {
        for(int j=100; j < 200; j++) {
            img.set_pixel(&img, (struct PointI16) { .x = i, .y = j}, (struct Pixel){ .r = 0, .g = 0, .b = 0 });
        }
    }
    img.save(&img, "ppm/hello2.ppm");
    free_image(&img);

    struct Image painter_img = make_image(400, 300);
    struct Painter painter = make_painter(&painter_img);

    painter.draw_point(&painter, (struct PointF32){ .x = 100, .y = 25 });
    
    painter.color = (struct Pixel){ .r = 255, .g = 0, .b = 0};
    painter.draw_point(&painter, (struct PointF32){ .x = 200.7, .y = 295.3 });
    painter.draw_line(&painter, (struct PointF32){0.5, 0}, (struct PointF32){200, 30.4});

    painter.draw_line(&painter, (struct PointF32){50.2, 100.6}, (struct PointF32){60, 250});
    painter.draw_circle(&painter, (struct PointF32){200.4, 150.1}, 100, 50);

    painter.color = (struct Pixel){ .r = 0, .g = 255, .b = 0};
    painter.draw_lines(&painter, (struct PointF32[]){{0, 0}, {10.3, 30}, {10, 50}}, 3);
    painter.draw_polygon(&painter, (struct PointF32[]){{50, 50.1}, {100, 50}, {100, 100}, {50, 100}}, 4);

    painter.color = (struct Pixel){ .r = 0, .g = 255, .b = 255};
    painter.draw_filled_triangle(&painter, (struct PointF32){150.3, 150}, (struct PointF32){230, 120.2}, (struct PointF32){200, 200});

    painter.color = (struct Pixel){ .r = 128, .g = 46, .b = 243};
    painter.draw_filled_circle(&painter, (struct PointF32){350.5, 75.2}, 30, 50);

    painter.color = (struct Pixel){ .r = 128, .g = 46, .b = 243};
    // bugged seems to be drawing wrong points?
    painter.draw_filled_polygon(&painter, (struct PointF32[]){{380, 225.25}, {359, 254}, {326, 243},
                                                              {326, 207}, {359, 196}}, 5);

    painter_img.save(&painter_img, "ppm/painter-test.ppm");

    return 0;
}