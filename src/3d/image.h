
#ifndef _GROOVER_IMAGE_H
#define _GROOVER_IMAGE_H

#include "gfx_types.h"

struct Image {

    uint8_t* pixels;
    uint16_t width;
    uint16_t height;

    // pixel array accessor functions
    struct Pixel (*get_pixel)(struct Image* img, struct Point2I16 pt);
    void (*set_pixel)(struct Image* img, struct Point2I16 pt, struct Pixel pix);

    // load a ppm file this will update the width and height accordingly
    void (*load)(struct Image* img, char* fname);
    // save current image to file
    void (*save)(struct Image* img, char* fname);

};

// Initialize an empty white image
struct Image make_image(uint16_t width, uint16_t height);
void free_image(struct Image* img);

#endif /* _GROOVER_IMAGE_H */