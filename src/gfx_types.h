
#ifndef _GROOVER_GFX_TYPES_H
#define _GROOVER_GFX_TYPES_H

#include <stdint.h>

struct PointI16 {
    int16_t x;
    int16_t y;
};

struct PointF32 {
    float x;
    float y;
};

// TODO: use this might be very verbose
struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#endif /* _GROOVER_GFX_TYPES_H */