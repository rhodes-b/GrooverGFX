
#ifndef _GROOVER_PAINTER_H
#define _GROOVER_PAINTER_H

#include "image.h"

struct Painter;
struct Painter {
    struct Pixel color;
    struct Image image;

    void (*draw_point)(struct Painter* painter, struct PointF32 pt);
    void (*draw_line)(struct Painter* painter, struct PointF32 p1, struct PointF32 p2);
    void (*draw_lines)(struct Painter* painter, struct PointF32* pts, uint16_t n_pts);
    void (*draw_polygon)(struct Painter* painter, struct PointF32* vertices, uint16_t n_vertices);
    void (*draw_circle)(struct Painter* painter, struct PointF32 center, uint16_t radius, uint16_t segments);
    void (*draw_filled_triangle)(struct Painter* painter, struct PointF32 p1, struct PointF32 p2, struct PointF32 p3);
    void (*draw_filled_polygon)(struct Painter* painter, struct PointF32* vertices, uint16_t n_vertices);
    void (*draw_filled_circle)(struct Painter* painter, struct PointF32 center, uint16_t radius, uint16_t segments);
};

struct Painter make_painter(struct Image* img);


#endif /* _GROOVER_PAINTEr_H */