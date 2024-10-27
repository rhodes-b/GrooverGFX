#include "render_oo.h"
#include "matrix.h"
#include "image.h"
#include <math.h>

#define MIN3(a, b, c) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    __typeof__(c) _c = (c); \
    _a < _b ? (_a < _c ? _a : _c) : (_b < _c ? _b : _c); \
})

#define MAX2(a, b) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b; \
})

#define MAX3(a, b, c) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    __typeof__(c) _c = (c); \
    _a > _b ? (_a > _c ? _a : _c) : (_b > _c ? _b : _c); \
})

struct FrameBuffer {
    struct Image img;
    struct Point2I16 window[2];

    float* depth_buff;
    struct Matrix transform;

    void (*draw_line)(struct FrameBuffer* fb, struct Point3F32 p1, struct Point3F32 p2, struct Pixel color);
    void (*draw_polygon)(struct FrameBuffer* fb, struct Point3F32* vertices, uint16_t n_vertices, struct Pixel color);

    void (*draw_phong_triangle)(struct FrameBuffer* fb, struct Point3F32 vertices[3], struct Pixel colors[3], float dots[3], float ambient);
    void (*draw_filled_triangle)(struct FrameBuffer* fb, struct Point3F32 vertices[3], struct Pixel colors[3]);
    struct Point3I16 (*trans_pt)(struct FrameBuffer* fb, struct Point3F32 p);
};

// TODO: this math is probably wrong to get the pixel value i forgot how image / painter did it before (plus this isn't a byte arr)
static inline float get_depth_buff(struct FrameBuffer* fb, struct Point2I16 pt) {
    return fb->depth_buff[((fb->img.height - pt.y - 1) * fb->img.width) + pt.x];
}

static inline void set_depth_buff(struct FrameBuffer* fb, struct Point2I16 pt, float z) {
    fb->depth_buff[((fb->img.height - pt.y - 1) * fb->img.width) + pt.x] = z;
}

static struct Point3I16 pix_loc(struct Point3F32 pt) {
    return (struct Point3I16) {
        .x = (uint16_t)roundf(pt.x),
        .y = (uint16_t)roundf(pt.y),
        .z = (uint16_t)roundf(pt.z),
    };
}

struct Point3I16 framebuffer_trans_pt(struct FrameBuffer* fb, struct Point3F32 p) {
    float res[4] = {0};
    float seq[4] = {p.x, p.y, p.z, 1.};
    matrix_apply(&fb->transform, &seq[0], 4, &res[0]);
    return pix_loc((struct Point3F32){res[0] + 0.5, res[1] + 0.5, res[2]});
}

static void set_pixel(struct FrameBuffer* fb, struct Point2I16 pt, float z, struct Pixel color) {
    if((0 <= pt.x && pt.x < fb->img.width) && (0 <= pt.y && pt.y < fb->img.height) && z > get_depth_buff(fb, pt)) {
        fb->img.set_pixel(&fb->img, pt, color.quantize(&color, 255));
        set_depth_buff(fb, pt, z);
    }
}

void framebuffer_draw_ln(struct FrameBuffer* fb, struct Point3F32 a, struct Point3F32 b, struct Pixel color) {
    struct Point3I16 a_rounded = pix_loc(a);
    struct Point3I16 b_rounded = pix_loc(b);
    if((a_rounded.x == b_rounded.x) && (a_rounded.y == b_rounded.y)) {
        set_pixel(fb, (struct Point2I16){a_rounded.x, a_rounded.y}, a.z, color);
    }

    int16_t dx = b_rounded.x - a_rounded.x;
    int16_t dy = b_rounded.y - a_rounded.y;
    float dz = b.z - a.z;
    if(abs(dx) >= abs(dy)) {
        if(a.x > b.x) {
            struct Point3I16 tmp = a_rounded;
            a_rounded = b_rounded;
            b_rounded = tmp;
        }
        float y = (float)a_rounded.y;
        float y_inc = (float)dy / dx;
        float z = a.z;
        float z_inc = dz / dx;
        for(int16_t x = a_rounded.x; x < b_rounded.x + 1; x++) {
            set_pixel(fb, (struct Point2I16){x, roundf(y)}, z, color);
            y += y_inc;
            z += z_inc;
        }
    }
    else {
        if(a.y > b.y) {
            struct Point3I16 tmp = a_rounded;
            a_rounded = b_rounded;
            b_rounded = tmp;
        }
        float x = (float)a_rounded.x;
        float x_inc = (float)dx / dy;
        float z = a.z;
        float z_inc = dz / dy;
        for(int16_t y = a_rounded.y; y < b_rounded.y + 1; y++) {
            set_pixel(fb, (struct Point2I16){roundf(x), y}, z, color);
            x += x_inc;
            z += z_inc;
        }
    }
}

void framebuffer_draw_poly(struct FrameBuffer* fb, struct Point3F32* vertices, uint16_t n_vertices, struct Pixel color) {
    struct Point3F32 pixels[n_vertices];
    for(uint16_t i=0; i < n_vertices; i++) {
        struct Point3I16 pt = fb->trans_pt(fb, vertices[i]);
        pixels[i] = (struct Point3F32){.x = pt.x, .y = pt.y, .z = pt.z};
    }
    for(uint16_t i=0; i < n_vertices-1; i++) {
        fb->draw_line(fb, pixels[i], pixels[i+1], color);
    }
    fb->draw_line(fb, pixels[n_vertices-1], pixels[0], color);
}

int16_t line_func(struct Point3I16 p0, struct Point3I16 p1, int16_t x, int16_t y) {
    return (p0.y-p1.y)*x + (p1.x-p0.x)*y + p0.x*p1.y-p1.x*p0.y;
}

void framebuffer_draw_phong_triangle(struct FrameBuffer* fb, struct Point3F32 vertices[3], struct Pixel colors[3], float dots[3], float ambient) {
    struct Point3I16 a = fb->trans_pt(fb, vertices[0]);
    struct Point3I16 b = fb->trans_pt(fb, vertices[1]);
    struct Point3I16 c = fb->trans_pt(fb, vertices[2]);

    int16_t fbc = line_func(b, c, a.x, a.y);
    int16_t fac = line_func(a, c, b.x, b.y);
    int16_t fab = line_func(a, b, c.x, c.y);
    // protect against div by 0
    if(fbc == 0 | fac == 0 | fab == 0) {
        return;
    }

    float alphamul = 1. / fbc;
    float betamul = 1. / fac;
    float gammamul = 1. / fab;

    for(uint16_t x = MIN3(a.x, b.x, c.x); x < MAX3(a.x, b.x, c.x) + 1; x++) {
        for(uint16_t y = MIN3(a.y, b.y, c.y); y < MAX3(a.y, b.y, c.y) + 1; y++) {
            float alpha = line_func(b, c, x, y) * alphamul;
            if(alpha < 0) continue;
            float beta = line_func(a, c, x, y) * betamul;
            if(beta < 0) continue;
            float gamma = line_func(a, b, x, y) * gammamul;
            if(gamma < 0) continue;

            // point is inside of the triangle
            float z = alpha*a.z + beta*b.z + gamma*c.z;
            float lambfact = MAX2(0.0, alpha*dots[0] + beta*dots[1] + gamma*dots[2]);
            struct Pixel ca = colors[0].mul(&colors[0], alpha);
            struct Pixel cb = colors[1].mul(&colors[1], beta);
            struct Pixel cc = colors[2].mul(&colors[2], gamma);
            struct Pixel tmp = ca.add(&ca, &cb);
            struct Pixel color = tmp.add(&tmp, &cc);
            struct Pixel lambfact_color = color.mul(&color, lambfact);
            struct Pixel ambient_color = color.mul(&color, ambient);
            struct Pixel final_color = lambfact_color.add(&lambfact_color, &ambient_color);
            set_pixel(fb, (struct Point2I16){x, y}, z, final_color);
        }
    }
}

void framebuffer_draw_filled_triangle(struct FrameBuffer *fb, struct Point3F32 vertices[3], struct Pixel colors[3]) {
    struct Point3I16 a = fb->trans_pt(fb, vertices[0]);
    struct Point3I16 b = fb->trans_pt(fb, vertices[1]);
    struct Point3I16 c = fb->trans_pt(fb, vertices[2]);

    int16_t fbc = line_func(b, c, a.x, a.y);
    int16_t fac = line_func(a, c, b.x, b.y);
    int16_t fab = line_func(a, b, c.x, c.y);
    // protect against div by 0
    if(fbc == 0 | fac == 0 | fab == 0) {
        return;
    }

    float alphamul = 1. / fbc;
    float betamul = 1. / fac;
    float gammamul = 1. / fab;

    for(uint16_t x = MIN3(a.x, b.x, c.x); x < MAX3(a.x, b.x, c.x) + 1; x++) {
        for(uint16_t y = MIN3(a.y, b.y, c.y); y < MAX3(a.y, b.y, c.y) + 1; y++) {
            float alpha = line_func(b, c, x, y) * alphamul;
            if(alpha < 0) continue;
            float beta = line_func(a, c, x, y) * betamul;
            if(beta < 0) continue;
            float gamma = line_func(a, b, x, y) * gammamul;
            if(gamma < 0) continue;

            // point is inside of the triangle
            float z = alpha*a.z + beta*b.z + gamma*c.z;
            struct Pixel ca = colors[0].mul(&colors[0], alpha);
            struct Pixel cb = colors[1].mul(&colors[1], beta);
            struct Pixel cc = colors[2].mul(&colors[2], gamma);
            struct Pixel tmp = ca.add(&ca, &cb);
            struct Pixel color = tmp.add(&tmp, &cc);
            set_pixel(fb, (struct Point2I16){x, y}, z, color);
        }
    }
}

struct FrameBuffer make_framebuffer(struct Image* img, struct Point2F32 window[2]) {
    struct FrameBuffer fb = {0};
    fb.img = *img;

    struct Matrix m = make_matrix(4, 4);

    uint16_t w = img->width + 1;
    uint16_t h = img->height + 1;
    float l, b, r, t;
    l = window[0].x;
    b = window[0].y;
    r = window[1].x;
    t = window[1].y;
    matrix_set(&m, 0, 0, w/(r-l));
    matrix_set(&m, 0, 3, (-.5*r-(w-.5)*l)/(r-l));
    matrix_set(&m, 1, 1, h/(t-b));
    matrix_set(&m, 1, 3, (-.5*t-(h-.5)*b)/(t-b));
    matrix_set(&m, 2, 2, 1.);
    matrix_set(&m, 3, 3, 1.);

    fb.transform = m;

    // TODO: this math is probably wrong (refer to comment above)
    fb.depth_buff = (float*)malloc(img->width*img->height * sizeof(float));
    for(uint32_t i=0; i < img->width*img->height; i++) {
        fb.depth_buff[i] = -INFINITY;
    }

    fb.draw_line = framebuffer_draw_ln;
    fb.draw_polygon = framebuffer_draw_poly;
    fb.trans_pt = framebuffer_trans_pt;
    fb.draw_phong_triangle = framebuffer_draw_phong_triangle;
    fb.draw_filled_triangle = framebuffer_draw_filled_triangle;

    return fb;
}

void render_wireframe(struct Scene* scene, struct Image* img) {
    struct FrameBuffer fb = make_framebuffer(img, scene->camera->window);
    int16_t d = -scene->camera->distance;
    struct Node* head = scene->objects.iter_polygons(&scene->objects);
    while(head->next != NULL) {
        struct Record r = head->data;
        struct Point3F32 pts[r.n_pts];
        for(uint16_t i=0; i < r.n_pts; i++) {
            pts[i] = (struct Point3F32){
                .x = d*r.pts[i].x / r.pts[i].z,
                .y = d*r.pts[i].y / r.pts[i].z,
                .z = r.pts[i].z,
            };
        }
        fb.draw_polygon(&fb, &pts[0], r.n_pts,  r.color);

        head = head->next;
    }
    free_nodes(head);   
    free_matrix(&fb.transform);
}

void render_signature(struct Scene* scene, struct Image* img) {
    struct FrameBuffer fb = make_framebuffer(img, scene->camera->window);
    int16_t d = -scene->camera->distance;
    struct Node* head = scene->objects.iter_polygons(&scene->objects);
    while(head->next != NULL) {
        struct Record r = head->data;
        struct Point3F32 pts[r.n_pts];
        for(uint16_t i=0; i < r.n_pts; i++) {
            pts[i] = (struct Point3F32){
                .x = d*r.pts[i].x / r.pts[i].z,
                .y = d*r.pts[i].y / r.pts[i].z,
                .z = r.pts[i].z,
            };
        }
        struct Pixel colors[3] = {r.color, r.color, r.color};
        for(uint8_t j=1; j < r.n_pts-1; j++) {
            fb.draw_filled_triangle(&fb, (struct Point3F32[3]){pts[0], pts[j], pts[j+1]}, colors);
        }

        head = head->next;
    }
    free_nodes(head);
    free_matrix(&fb.transform);
}

void render_phong(struct Scene* scene, struct Image* img) {
    struct FrameBuffer fb = make_framebuffer(img, scene->camera->window);
    int16_t d = -scene->camera->distance;
    struct Node* head = scene->objects.iter_polygons(&scene->objects);
    while(head->next != NULL) {
        struct Record r = head->data;
        struct Point3F32 pts[r.n_pts];
        struct Vec3 lvecs[r.n_pts];
        float dots[r.n_pts];
        for(uint16_t i=0; i < r.n_pts; i++) {
            pts[i] = (struct Point3F32){
                .x = d*r.pts[i].x / r.pts[i].z,
                .y = d*r.pts[i].y / r.pts[i].z,
                .z = r.pts[i].z,
            };
            lvecs[i] = point3F32_sub(&scene->light, &pts[i]);
            vec3_normalize(&lvecs[i]);
            dots[i] = vec3_dot(&lvecs[i], &r.normal);
        }
        struct Pixel colors[3] = {r.color, r.color, r.color};
        for(uint8_t j=1; j < r.n_pts-1; j++) {
            fb.draw_phong_triangle(
                &fb,
                (struct Point3F32[3]){pts[0], pts[j], pts[j+1]},
                colors,
                (float[3]){dots[0], dots[j], dots[j+1]},
                scene->ambient
            );
        }

        head = head->next;
    }
    free_nodes(head);
    free_matrix(&fb.transform);
}

void render_gourad(struct Scene* scene, struct Image* img);
