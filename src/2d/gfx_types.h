
#ifndef _GROOVER_GFX_TYPES_H
#define _GROOVER_GFX_TYPES_H

#include <stdint.h>
#include <math.h>

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

static inline float degrees_to_radians(float degrees) {
    return degrees * (M_PI / 180.0);
}

#endif /* _GROOVER_GFX_TYPES_H */