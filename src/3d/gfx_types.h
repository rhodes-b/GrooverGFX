
#ifndef _GROOVER_GFX_TYPES_H
#define _GROOVER_GFX_TYPES_H

#include <stdint.h>

struct Point2I16 {
    int16_t x;
    int16_t y;
    int16_t z;
};

struct Point2F32 {
    float x;
    float y;
    float z;
};

struct Point3I16 {
    int16_t x;
    int16_t y;
    int16_t z;
};

struct Point3F32 {
    float x;
    float y;
    float z;
};

// TODO: use this might be very verbose
struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#endif /* _GROOVER_GFX_TYPES_H */