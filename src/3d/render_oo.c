
#include "render_oo.h"
#include "matrix.h"
#include <math.h>

struct FrameBuffer {
    struct Image img;
    struct Point2I16 window[2];

    float* depth_buff;

    struct Matrix transform;

    void (*draw_line)(struct FrameBuffer* fb, struct Point2I16 p1, struct Point2I16 p2, struct Pixel color);
    void (*draw_polygon)(struct FrameBuffer* fb, struct Point2F32* vertices, uint16_t n_vertices, struct Pixel color);
    struct Point2I16 (*trans_pt)(struct FrameBuffer* fb, struct Point2F32 p);
};

static struct Point2I16 pix_loc(struct Point2F32 pt) {
    return (struct Point2I16) {
        .x = (uint16_t)roundf(pt.x),
        .y = (uint16_t)roundf(pt.y),
    };
}

static void draw_pt(struct FrameBuffer* buff, struct Point2F32 pt, struct Pixel color) {
    struct Point2I16 rounded_pt = pix_loc(pt);
    if((rounded_pt.x >= buff->window[0].x) &&
       (rounded_pt.x <= buff->window[1].x) &&
       (rounded_pt.y >= buff->window[0].y) &&
       (rounded_pt.y <= buff->window[1].y)) {
        buff->img.set_pixel(&buff->img, rounded_pt, color);
       }
}

// TODO: this math is probably wrong to get the pixel value i forgot how image / painter did it before (plus this isn't a byte arr)
static inline float get_depth_buff(struct FrameBuffer* fb, struct Point2I16 pt) {
    return fb->depth_buff[(pt.x * (fb->img.width-1)) + pt.y];
}

static inline void set_depth_buff(struct FrameBuffer* fb, struct Point2I16 pt, float z) {
    fb->depth_buff[(pt.x * (fb->img.width-1)) + pt.y] = z;
}


void framebuffer_draw_ln(struct FrameBuffer* fb, struct Point2I16 a, struct Point2I16 b, struct Pixel color) {
    if((a.x == b.x) && (a.y == b.y)) {
        fb->img.set_pixel(&fb->img, a, color);
    }

    int16_t dx = b.x - a.x;
    int16_t dy = b.y - a.y;
    if(abs(dx) >= abs(dy)) {
        if(a.x > b.x) {
            struct Point2I16 tmp = a;
            a = b;
            b = tmp;
        }
        float y = (float)a.y;
        float y_inc = (float)dy / dx;
        for(int16_t x = a.x; x < b.x + 1; x++) {
            fb->img.set_pixel(&fb->img, (struct Point2I16){x, y}, color);
            y += y_inc;
        }
    }
    else {
        if(a.y > b.y) {
            struct Point2I16 tmp = a;
            a = b;
            b = tmp;
        }
        float x = (float)a.x;
        float x_inc = (float)dx / dy;
        for(int16_t y = a.y; y < b.y + 1; y++) {
            fb->img.set_pixel(&fb->img, (struct Point2I16){x, y}, color);
            x += x_inc;
        }
    }
}

void framebuffer_draw_poly(struct FrameBuffer* fb, struct Point2F32* vertices, uint16_t n_vertices, struct Pixel color) {
    struct Point2I16 pixels[n_vertices];
    for(uint16_t i=0; i < n_vertices; i++) {
        pixels[i] = fb->trans_pt(fb, vertices[i]);
    }
    for(uint16_t i=0; i < n_vertices-1; i++) {
        fb->draw_line(fb, pixels[i], pixels[i+1], color);
    }
    fb->draw_line(fb, pixels[n_vertices-1], pixels[0], color);
}

struct Point2I16 framebuffer_trans_pt(struct FrameBuffer* fb, struct Point2F32 p) {
    float res[3];
    float seq[3] = {p.x, p.y, 1.};
    matrix_apply(&fb->transform, &seq[0], 3, &res[0]);
    return pix_loc((struct Point2F32){res[0], res[1]});
}

struct FrameBuffer make_framebuffer(struct Image* img, struct Point2F32 window[2]) {
    struct FrameBuffer fb = {0};
    fb.img = *img;

    struct Matrix m = make_matrix(3, 3);

    uint16_t w = img->width + 1;
    uint16_t h = img->height + 1;
    float l, b, r, t;
    l = window[0].x;
    b = window[0].y;
    r = window[1].x;
    t = window[1].y;
    matrix_set(&m, 0, 0, w/(r-l));
    matrix_set(&m, 0, 2, (-.5*r-(w-.5)*l)/(r-l));
    matrix_set(&m, 1, 1, h/(t-b));
    matrix_set(&m, 1, 2, (-.5*t-(h-.5)*b)/(t-b));
    matrix_set(&m, 2, 2, 1.);

    fb.transform = m;

    // TODO: this math is probably wrong (refer to comment above)
    fb.depth_buff = (float*)malloc(img->width*img->height * sizeof(float));
    for(uint16_t i=0; i < img->width; i++) {
        for(uint16_t j=0; j < img->height; j++) {
            fb.depth_buff[(i * (img->width - 1)) + j] = -INFINITY;
        }
    }

    fb.draw_line = framebuffer_draw_ln;
    fb.draw_polygon = framebuffer_draw_poly;
    fb.trans_pt = framebuffer_trans_pt;

    return fb;
}

void render_wireframe(struct Scene* scene, struct Image* img) {
    struct FrameBuffer fb = make_framebuffer(img, scene->camera->window);
    struct Node* head = scene->objects.iter_polygons(&scene->objects);
    while(head->next != NULL) {
        struct Record r = head->data;

        struct Point2F32 pts[r.n_pts];
        for(uint16_t i=0; i < r.n_pts; i++) {
            pts[i] = (struct Point2F32){
                .x = -scene->camera->distance*(r.pts[i].x / r.pts[i].z),
                .y = -scene->camera->distance*(r.pts[i].y / r.pts[i].z),
            };
        }
        fb.draw_polygon(&fb, &pts[0], r.n_pts,  r.color.quantize(&r.color, 255));

        head = head->next;
    }
    free_nodes(head);
    free_matrix(&fb.transform);
}

void render_signature(struct Scene* scene, struct Image* img);

void render_phong(struct Scene* scene, struct Image* img);

void render_gourad(struct Scene* scene, struct Image* img);
