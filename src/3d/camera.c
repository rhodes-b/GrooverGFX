
#include "camera.h"
#include "matrix.h"

void set_cam_perspective(struct Camera* c, float hfov, float aspect, uint16_t distance) {
    c->distance = distance;
    float half_width = tan(degrees_to_radians(hfov)/2) * distance;
    c->window[0].x = -half_width;
    c->window[0].y = -half_width / aspect;
    c->window[1].x = half_width;
    c->window[1].y = half_width / aspect;
}

void set_cam_res(struct Camera* c, float width, float height) {
    c->dx = (c->window[1].x - c->window[0].x) / width;
    c->dy = (c->window[1].y - c->window[0].y) / height;
}

struct Ray ij_ray(struct Camera* c, float i, float j) {

    struct Point3F32 pt = {
        c->window[0].x + c->dx*(.5f + i),
        c->window[0].y + c->dy*(.5f + j),
    };
    struct Vec3 v = point3F32_sub(&pt , &c->eye);
    return (struct Ray){c->eye, v};
}

struct Camera make_camera() {
    return (struct Camera) {
        .eye = {0},
        .window = {{-10, -10}, {10, 10}},
        .distance = 10,
        .dx = 0,
        .dy = 0,
        .set_perspective = set_cam_perspective,
        .set_resolution = set_cam_res,
        .ij_ray = ij_ray,
    };
}
