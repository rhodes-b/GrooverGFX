
#ifndef _GROOVER_RAY3D_H
#define _GROOVER_RAY3D_H

#include "math3d.h"
#include <stdbool.h>

struct Ray {
    struct Point3F32 start;
    struct Vec3 dir;

    struct Point3F32 (*point_at)(struct Ray* r, float t);
};

struct Interval {
    float low;
    float high;

    bool (*contains)(struct Interval* i, float t);
    void (*set)(struct Interval* i, float low, float high);
};

struct Ray make_ray(struct Point3F32 start, struct Vec3 dir);

struct Interval make_interval(float low, float high);

#endif /* _GROOVER_RAY3D_H */