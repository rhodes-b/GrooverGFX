
#ifndef _GROOVER_CAMERA_H
#define _GROOVER_CAMERA_H

#include "gfx_types.h"
#include "math3d.h"
#include "ray3d.h"

struct Camera;
struct Camera {
    struct Point3F32 eye;
    struct Point2F32 window[2];
    uint16_t distance;
    float dx;
    float dy;

    void (*set_perspective)(struct Camera* cam, float hfov, float aspect, uint16_t distance);
    void (*set_resolution)(struct Camera* cam, uint16_t width, uint16_t height);
    struct Ray (*ij_ray)(struct Camera* cam, float i, float j);
};

struct Camera make_camera();

#endif /* _GROOVER_CAMERA_H */