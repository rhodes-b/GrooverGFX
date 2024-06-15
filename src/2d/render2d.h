
#ifndef _GROOVER_RENDER2D_H
#define _GROOVER_RENDER2D_H

#include "gfx_types.h"
#include "painter.h"
#include "matrix.h"

struct Render2d;
struct Render2d {
    struct Image image;
    struct Painter painter;
    struct Matrix transform;
    struct Matrix* trans_stack[100]; // we shouldn't ever need more than this?
    uint8_t n_transforms;
    struct PointI16 window[2];

    struct Pixel (*get_color)(struct Render2d* render);
    void (*set_color)(struct Render2d* render, struct Pixel c);
    void (*get_viewport)(struct Render2d* render, struct PointI16* p1, struct PointI16* p2);
    void (*loadview)(struct Render2d* render, struct PointI16 window[2], struct PointI16 viewport[2] /* = NULL */);
    void (*push_matrix)(struct Render2d* render, struct Matrix* m);
    struct Matrix* (*pop_matrix)(struct Render2d* render);
    void (*scale)(struct Render2d* render, float sx, float sy);
    void (*rotate)(struct Render2d* render, float angle);
    void (*translate)(struct Render2d* render, float dx, float dy);
    void (*shear)(struct Render2d* render, float sx, float sy);
    void (*reflect_x)(struct Render2d* render);
    void (*reflect_y)(struct Render2d* render);
    void (*point)(struct Render2d* render, struct PointF32 pt);
    void (*line)(struct Render2d* render, struct PointF32 p1, struct PointF32 p2);
    void (*lines)(struct Render2d* render, struct PointF32* pts, uint16_t n_pts);
    void (*polygon)(struct Render2d* render, struct PointF32* vertices, uint16_t n_vertices);
    void (*circle)(struct Render2d* render, struct PointF32 center, float radius, uint16_t segments);
    void (*filled_triangle)(struct Render2d* render, struct PointF32 p1, struct PointF32 p2, struct PointF32 p3);
    void (*filled_polygon)(struct Render2d* render, struct PointF32* vertices, uint16_t n_vertices);
    void (*filled_circle)(struct Render2d* render, struct PointF32 center, float radius, uint16_t segments);

};

struct Render2d make_render2d(uint16_t width, uint16_t height);


#endif /* _GROOVER_RENDER2D_H */