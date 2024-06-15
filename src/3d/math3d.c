
#include "math3d.h"
#include <math.h>

struct Vec2 vec2_mul(struct Vec2* v, float s) {
    return make_vec2(v->x*s, v->y*s);
}

struct Vec3 vec3_mul(struct Vec3* v, float s) {
    return make_vec3(v->x*s, v->y*s, v->z*s);
}

struct Point2F32 vec2_add_pt(struct Vec2* v, struct Point2F32* p) {
    return (struct Point2F32){v->x + p->x, v->y + p->y};
}

struct Point3F32 vec3_add_pt(struct Vec3* v, struct Point3F32* p) {
    return (struct Point3F32){v->x + p->x, v->y + p->y, v->z + p->z};
}

struct Vec2 vec2_add(struct Vec2* v1, struct Vec2* v2) {
    return make_vec2(v1->x + v2->x, v1->y + v2->y);
}

struct Vec3 vec3_add(struct Vec3* v1, struct Vec3* v2) {
    return make_vec3(v1->x + v2->x, v1->y + v2->y, v1->z + v2->z);
}

struct Vec2 vec2_neg(struct Vec2* v) {
    return make_vec2(-v->x, -v->y);
}

struct Vec3 vec3_neg(struct Vec3* v) {
    return make_vec3(-v->x, -v->y, -v->z);
}

struct Vec2 vec2_sub(struct Vec2* v1, struct Vec2* v2) {
    return make_vec2(v1->x - v2->x, v1->y - v2->y);
}

struct Vec3 vec3_sub(struct Vec3* v1, struct Vec3* v2) {
    return make_vec3(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
}

float vec2_dot(struct Vec2* v1, struct Vec2* v2) {
    return v1->x * v2->x + v1->y + v2->y;
}

float vec3_dot(struct Vec3* v1, struct Vec3* v2) {
    return v1->x * v2->x + v1->y + v2->y + v1->z * v2->z;
}

struct Vec3 vec3_cross(struct Vec3* v1, struct Vec3* v2) {
    return make_vec3(
        v1->y * v2->z - v2->y * v1->z,
        v2->x * v1->z - v1->x * v2->z,
        v1->x * v2->y - v2->x * v1->y
    );
}

float vec2_mag_sq(struct Vec2* v) {
    return v->x * v->x + v->y * v->y;
}

float vec3_mag_sq(struct Vec3* v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

float vec2_mag(struct Vec2* v) {
    return sqrtf(v->x * v->x + v->y * v->y);
}

float vec3_mag(struct Vec3* v) {
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

void vec2_normalize(struct Vec2* v) {
    float mag = vec2_mag(v);
    v->x /= mag;
    v->y /= mag;
}

void vec3_normalize(struct Vec3* v) {
    float mag = vec3_mag(v);
    v->x /= mag;
    v->y /= mag;
    v->z /= mag;
}

struct Vec2 vec2_normalized(struct Vec2* v) {
    float mag = vec2_mag(v);
    return make_vec2(v->x / mag, v->y / mag);
}

struct Vec3 vec3_normalized(struct Vec3* v) {
    float mag = vec3_mag(v);
    return make_vec3(v->x / mag, v->y / mag, v->z / mag);
}

struct Vec2 point2F32_sub(struct Point2F32* p1, struct Point2F32* p2) {
    return make_vec2(p1->x - p2->x, p1->y - p2->y);
}

struct Vec3 point3F32_sub(struct Point3F32* p1, struct Point3F32* p2) {
    return make_vec3(p1->x - p2->x, p1->y - p2->y, p1->z - p2->z);
}

struct Point2F32 point2F32_sub_vec2(struct Point2F32* p, struct Vec2* v) {
    return (struct Point2F32){p->x - v->x, p->y - v->y};
}

struct Point3F32 point3F32_sub_vec2(struct Point3F32* p, struct Vec3* v) {
    return (struct Point3F32){p->x - v->x, p->y - v->y, p->z - v->z};
}


float vec2_get(struct Vec2* v, uint8_t pos) {
    return pos == 0 ? v->x : v->y;
}

float vec3_get(struct Vec3* v, uint8_t pos) {
    return pos == 0 ? v->x : pos == 1 ? v->y : v->z;
}

void vec2_set(struct Vec2* v, uint8_t pos, float val) {
    switch(pos) {
        case 0:
            v->x = val;
            break;
        case 1:
            v->y = val;
            break;
    }
}

void vec3_set(struct Vec3* v, uint8_t pos, float val) {
    switch(pos) {
        case 0:
            v->x = val;
            break;
        case 1:
            v->y = val;
            break;
        case 2:
            v->z = val;
            break;
    }
}

struct Vec2 make_vec2(float v1, float v2) {
    return (struct Vec2) {
        .x = v1,
        .y = v2,
        .get_pos = vec2_get,
        .set_pos = vec2_set,
    };
}

struct Vec3 make_vec3(float v1, float v2, float v3) {
    return (struct Vec3) {
        .x = v1,
        .y = v2,
        .z = v3,
        .get_pos = vec3_get,
        .set_pos = vec3_set,
    };
}