
#include "gfx_types.h"

static struct Pixel quantize_pixel(struct Pixel* p, uint8_t val) {
    return make_pixel(
        roundf(p->r * val),
        roundf(p->g * val),
        roundf(p->b * val)
    );
}

struct Pixel make_pixel(float r, float g, float b) {
    return (struct Pixel) {
        .r = r,
        .g = g,
        .b = b,
        .quantize = quantize_pixel,
    };
}