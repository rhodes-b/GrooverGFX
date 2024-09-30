
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

struct Pixel mul_pixel(struct Pixel *p, float val) {
    return make_pixel(
        p->r * val,
        p->g * val,
        p->b * val
    );
}
struct Pixel add_pixel(struct Pixel *p, struct Pixel* other) {
    return make_pixel(
        p->r + other->r,
        p->g + other->g,
        p->b + other->b
    );
}

struct Pixel make_pixel(float r, float g, float b) {
    return (struct Pixel) {
        .r = r,
        .g = g,
        .b = b,
        .quantize = quantize_pixel,
        .mul = mul_pixel,
        .add = add_pixel,
    };
}