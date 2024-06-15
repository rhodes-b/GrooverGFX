
#include <image.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <render2d.h>

void image_test() {
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
}

void painter_test() {
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
    
    painter.draw_filled_polygon(&painter, (struct PointF32[]){{380, 225.25}, {359, 254}, {326, 243},
                                                              {326, 207}, {359, 196}}, 5);

    painter_img.save(&painter_img, "ppm/painter-test.ppm");
}

void read_until_tok_or_eof(int fd, char* buff, char tok) {
    ssize_t bytes_read;
    char ch;
    uint8_t i = 0;
    while(1) {
        if(bytes_read = read(fd, &ch, 1) == 0) {
            buff[i] = '\0';
            break;
        }
        if (ch == tok) {
            buff[i] = '\0';
            break;
        }
        buff[i++] = ch;
    }
}

void draw_dino() {

    struct Image img = make_image(635, 440);
    struct Painter p = make_painter(&img);

    int fd = open("ppm/drawing.dat", O_RDONLY);

    char buffer[10];
    read_until_tok_or_eof(fd, &buffer[0], '\n');
    uint8_t segments = (uint8_t)strtol(buffer, NULL, 0);
    memset(buffer, 0, sizeof(buffer));

    for(int i=0; i < segments; i++) {
        read_until_tok_or_eof(fd, &buffer[0], '\n');
        uint8_t curr_segment_points = (uint8_t)strtol(buffer, NULL, 0);
        memset(buffer, 0, sizeof(buffer));
        struct PointF32* points = (struct PointF32*)malloc(curr_segment_points*sizeof(struct PointF32));
        for(uint8_t j=0; j < curr_segment_points; j++) {
            read_until_tok_or_eof(fd, &buffer[0], ' ');
            float x = strtol(buffer, NULL, 0);
            memset(buffer, 0, sizeof(buffer));
            points[j].x = x;

            read_until_tok_or_eof(fd, &buffer[0], '\n');
            float y = strtof(buffer, NULL);
            memset(buffer, 0, sizeof(buffer));
            points[j].y = y;
        }
        p.draw_lines(&p, points, curr_segment_points);
        free(points);
    }

    p.image.save(&p.image, "ppm/dino.ppm");
}

void render_test() {
   
    // TODO: this isnt working :(
    struct Render2d r = make_render2d(400, 300);
    r.loadview(&r, (struct PointI16[]){{0, 0}, {3, 3}}, NULL);
    // r.line(&r, (struct PointF32){1, 0}, (struct PointF32){1, 3});
    // r.line(&r, (struct PointF32){2, 0}, (struct PointF32){2, 3});
    // r.line(&r, (struct PointF32){0, 1}, (struct PointF32){3, 1});
    // r.line(&r, (struct PointF32){0, 2}, (struct PointF32){3, 2});
    // r.filled_polygon(&r, (struct PointF32[]){{1, 1}, {1, 2}, {2, 2}, {2, 1}}, 4);
    // r.set_color(&r, (struct Pixel){255, 0, 0});
    // r.filled_circle(&r, (struct PointF32){1.5, 1.5}, 0.5, 50);
    r.image.save(&r.image, "ppm/window_test.ppm");
}

int main() {

    // image_test();
    // painter_test();
    // draw_dino();
    // render_test();

    return 0;
}