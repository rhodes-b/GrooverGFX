
#ifndef _GROOVER_IMAGE_H
#define _GROOVER_IMAGE_H

#include <stdint.h>


struct Point {
    uint16_t x;
    uint16_t y;
};

// TODO: use this might be very verbose
struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct Image;
struct Image {

    uint8_t* pixels;
    uint16_t width;
    uint16_t height;

    struct Pixel (*get_pixel)(struct Image* img, struct Point pt);
    void (*set_pixel)(struct Image* img, struct Point pt, uint8_t r, uint8_t g, uint8_t b);

    void (*load)(struct Image* img, char* fname);
    void (*save)(struct Image* img, char* fname);

};

// Initialize an empty white image
struct Image make_image(uint16_t width, uint16_t height);
void free_image(struct Image* img);

#endif /* _GROOVER_IMAGE_H */