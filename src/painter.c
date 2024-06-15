
#include <math.h>
#include "painter.h"
#include <stdlib.h>

#define MIN(a, b) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b; \
})

#define MAX(a, b) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b; \
})

#define M_TAU (M_PI * 2)

static struct PointI16 pix_loc(struct PointF32 pt) {
    return (struct PointI16) {
        .x = (uint16_t)roundf(pt.x),
        .y = (uint16_t)roundf(pt.y),
    };
}

static void draw_pt(struct Painter* painter, struct PointF32 pt) {
    struct PointI16 rounded_pt = pix_loc(pt);
    if((rounded_pt.x >= painter->viewport[0].x) && 
       (rounded_pt.x <= painter->viewport[1].x) &&
       (rounded_pt.y >= painter->viewport[0].y) &&
       (rounded_pt.y <= painter->viewport[1].y)
      ) {
        painter->image.set_pixel(&painter->image, rounded_pt, painter->color);
       }
}

static void draw_ln(struct Painter* painter, struct PointF32 p1, struct PointF32 p2) {
    struct PointI16 a = pix_loc(p1); 
    struct PointI16 b = pix_loc(p2);
    if((a.x == b.x) && (a.y == b.y)) {
        painter->draw_point(painter, p1);
        return;
    }

    int16_t dx = b.x - a.x; 
    int16_t dy = b.y - a.y;
    if(abs(dx) >= abs(dy)) {
        if(a.x > b.x) {
            struct PointI16 tmp = a;
            a = b;
            b = tmp;
        }
        float y = (float)a.y;
        float y_inc = (float)dy / dx;
        for(int16_t x = a.x; x < b.x + 1; x++) {
            painter->draw_point(painter, (struct PointF32){ .x = (float)x, .y = y});
            y += y_inc;
        }
    }
    else {
        if(a.y > b.y) {
            struct PointI16 tmp = a;
            a = b;
            b = tmp;
        }
        float x = (float)a.x;
        float x_inc = (float)dx / dy;
        for(int16_t y = a.y; y < b.y + 1; y++) {
            painter->draw_point(painter, (struct PointF32){ .x = x, .y = (float)y});
            x += x_inc;
        }
    }
}

static void draw_lns(struct Painter* painter, struct PointF32* pts, uint16_t n_pts) {
    for(uint16_t i=0; i < n_pts-1; i++) {
        painter->draw_line(painter, pts[i], pts[i+1]);
    }
} 

static void draw_poly(struct Painter* painter, struct PointF32* vertices, uint16_t n_vertices) {
    painter->draw_lines(painter, vertices, n_vertices);
    painter->draw_line(painter, vertices[0], vertices[n_vertices-1]);
}

void circle_points(struct PointF32 center, float radius, uint16_t segments, struct PointF32* pts) {
   float dtheta = M_TAU / segments;
   float theta = 0.0;

    for(uint16_t i=0; i < segments; i++) {
        float x =  center.x + radius * cos(theta);
        float y =  center.y + radius * sin(theta);
        pts[i] = (struct PointF32){ .x = x, .y = x };
    }
}

static void draw_circ(struct Painter* painter, struct PointF32 center, uint16_t radius, uint16_t segments) {
    struct PointF32 pts[segments];
    circle_points(center, radius, segments, &pts[0]);
    painter->draw_polygon(painter, pts, segments);
}

static inline int line_fn(struct PointI16 p1, struct PointI16 p2, struct PointI16 p3) {
    return (p1.y - p2.y) * p3.x + (p2.x - p1.x) * p3.y + p1.x * p2.y - p2.x * p1.y;
}

static void barycentric_draw(struct Painter* painter, struct PointF32 p1, struct PointF32 p2, struct PointF32 p3) {
    struct PointI16 a = pix_loc(p1);
    struct PointI16 b = pix_loc(p2);
    struct PointI16 c = pix_loc(p3);

    int16_t min_x = MIN(a.x, b.x);
    min_x = MIN(min_x, c.x);

    int16_t max_x = MAX(a.x, b.x);
    max_x = MAX(max_x, c.x);

    int16_t min_y = MIN(a.y, b.y);
    min_y = MIN(min_y, c.y);

    int16_t max_y = MAX(a.y, b.y);
    max_y = MAX(max_y, c.y);

    for(int16_t x=min_x; x < max_x+1; x++) {
        for(int16_t y=min_y; y < max_y+1; y++) {
            float alpha = (float)line_fn(b, c, (struct PointI16){ .x = x, .y = y}) / line_fn(b, c, a);
            float beta = (float)line_fn(a, c, (struct PointI16){ .x = x, .y = y}) / line_fn(a, c, b);
            float gamma = (1. - alpha - beta);
            if((alpha >= 0) && (beta >= 0) && (gamma >= -0.0000001)) {
                painter->draw_point(painter, (struct PointF32){ .x = (float)x, .y = (float)y });
            }
        }
    }
}

static void draw_filled_tri(struct Painter* painter, struct PointF32 p1, struct PointF32 p2, struct PointF32 p3) {
    painter->draw_polygon(painter, (struct PointF32[]){p1, p2, p3}, 3);
    barycentric_draw(painter, p1, p2, p3);
}

static void draw_filled_poly(struct Painter* painter, struct PointF32* vertices, uint16_t n_vertices) {
    struct PointI16 pts[n_vertices];
    int sum_x = 0, sum_y = 0;
    for(uint16_t i=0; i < n_vertices; i++) {
        pts[i] = pix_loc(vertices[i]);
        sum_x += pts[i].x;
        sum_y += pts[i].y;
    }

    float avg_x = (float)sum_x / n_vertices;
    float avg_y = (float)sum_y / n_vertices;

    for(int16_t i=0; i < n_vertices-1; i++) {
        painter->draw_filled_triangle(painter, (struct PointF32){ .x = avg_x, .y = avg_y}, vertices[i], vertices[i+1]);
    }
    painter->draw_filled_triangle(painter, (struct PointF32){ .x = avg_x, .y = avg_y}, vertices[0], vertices[n_vertices-1]);

}

static void draw_filled_circ(struct Painter* painter, struct PointF32 center, float radius, uint16_t segments) {
    struct PointF32 pts[segments];
    circle_points(center, radius, segments, pts);
    for(int16_t i=0; i < segments - 1; i++) {
        painter->draw_filled_triangle(painter, center, pts[i], pts[i+1]);
    }
    painter->draw_filled_triangle(painter, center, pts[0], pts[segments-1]);
}

struct Painter make_painter(struct Image* img) {
    return (struct Painter) {
        .color = (struct Pixel){ .r = 0, .g = 0, .b = 0},
        .image = *img,
        .viewport = {{.x = 0, .y = 0}, {.x = img->width-1, .y = img->height-1}},
        .draw_point = draw_pt,
        .draw_line = draw_ln,
        .draw_lines = draw_lns,
        .draw_polygon = draw_poly,
        .draw_circle = draw_circ,
        .draw_filled_triangle = draw_filled_tri,
        .draw_filled_polygon = draw_filled_poly,
        .draw_filled_circle = draw_filled_circ,
    };
}
