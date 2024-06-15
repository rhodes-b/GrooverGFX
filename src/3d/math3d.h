
#ifndef _GROOVER_MATH3D_H
#define _GROOVER_MATH3D_H

#include <stdint.h>
#include "gfx_types.h"

struct Vec2 {
    float x;
    float y;

    void (*set_pos)(struct Vec2* v, uint8_t pos, float val);
    float (*get_pos)(struct Vec2* v, uint8_t pos);
};

struct Vec3 {
    float x;
    float y;
    float z;

    void (*set_pos)(struct Vec3* v, uint8_t pos, float val);
    float (*get_pos)(struct Vec3* v, uint8_t pos);
};

struct Vec2 make_vec2(float v1, float v2);
struct Vec3 make_vec3(float v1, float v2, float v3);

struct Vec2 vec2_mul(float s);
struct Vec3 vec3_mul(float s);

struct Point3F32 vec2_add_pt(struct Vec2* v, struct Point3F32 p);
struct Point3F32 vec3_add_pt(struct Vec3* v, struct Point3F32 p);

struct Point3F32 vec2_add(struct Vec2* v1, struct Vec2* v2);
struct Point3F32 vec3_add(struct Vec3* v1, struct Vec3* v2);

struct Vec2 vec2_neg(struct Vec2* v);
struct Vec3 vec3_neg(struct Vec3* v);

struct Point3F32 vec2_sub(struct Vec2* v1, struct Vec2* v2);
struct Point3F32 vec3_sub(struct Vec3* v1, struct Vec3* v2);

float vec2_dot(struct Vec2* v1, struct Vec2* v2);
float vec3_dot(struct Vec3* v1, struct Vec3* v2);

float vec2_cross(struct Vec2* v1, struct Vec2* v2);
float vec3_cross(struct Vec3* v1, struct Vec3* v2);

float vec2_mag_sq(struct Vec2* v);
float vec3_mag_sq(struct Vec3* v);

float vec2_mag(struct Vec2* v);
float vec3_mag(struct Vec3* v);

void vec2_normalize(struct Vec2* v);
void vec3_normalize(struct Vec3* v);

struct Vec2 vec2_normalized(struct Vec2* v);
struct Vec3 vec3_normalized(struct Vec3* v);

struct Vec2 point2F32_sub(struct Point2F32* p1, struct Point2F32* p2);
struct Vec3 point3F32_sub(struct Point3F32* p1, struct Point3F32* p2);

struct Point2F32 point2F32_sub_vec2(struct Point2F32* p, struct Vec2* v);
struct Point3F32 point3F32_sub_vec2(struct Point3F32* p, struct Vec3* v);

#endif  /* _GROOVER_MATH3D_H */