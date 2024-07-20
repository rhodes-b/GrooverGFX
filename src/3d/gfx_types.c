
#include "gfx_types.h"

#define MIN(a, b) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b; \
})

static struct Pixel quantize_pixel(struct Pixel* p, uint8_t val) {
    return make_pixel(
        MIN(roundf(p->r * val), val),
        MIN(roundf(p->g * val), val),
        MIN(roundf(p->b * val), val)
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