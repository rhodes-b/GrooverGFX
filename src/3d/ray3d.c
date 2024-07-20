
#include "ray3d.h"

static struct Point3F32 ray_point_at(struct Ray* r, float t) {
    struct Vec3 v = vec3_mul(&r->dir, t);
    return vec3_add_pt(&v, &r->start);
}

static bool interval_contains(struct Interval* i, float t) {
    return (i->low < t) && (t < i->high);
}

static void interval_set(struct Interval* i, float low, float high) {
    i->low = low;
    i->high = high;
}

struct Ray make_ray(struct Point3F32 start, struct Vec3 dir) {
    return (struct Ray) {
        .start = start,
        .dir = dir,
        .point_at = ray_point_at,
    };
}

struct Interval make_interval(float low, float high) {
    return (struct Interval) {
        .low = low,
        .high = high,
        .contains = interval_contains,
        .set = interval_set,
    };
}