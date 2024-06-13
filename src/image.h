
#ifndef _GROOVER_IMAGE_H
#define _GROOVER_IMAGE_H

#include <stdint.h>

struct Image;

struct Image {

    uint8_t* pixels;
    uint16_t width;
    uint16_t height;

    void (*load)(struct Image* img, char* fname);
    void (*save)(struct Image* img, char* fname);

};

// Makes a empty white image
struct Image make_image(uint16_t width, uint16_t height);
void free_image(struct Image* img);

#endif /* _GROOVER_IMAGE_H */