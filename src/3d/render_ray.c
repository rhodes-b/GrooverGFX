
#include "render_ray.h"

#define MAX(a, b) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b; \
})

struct Pixel raycolor(struct Scene* scene, struct Ray* r, struct Interval* interval) {
    struct Record info = {0};
    struct Pixel color = make_pixel(0, 0, 0);
    if(scene->objects.intersect(&scene->objects, r, interval, &info)) {
        struct Point3F32 tmp = r->point_at(r, info.time);
        struct Vec3 lvec = point3F32_sub(&scene->camera->eye, &tmp);
        lvec = vec3_normalized(&lvec);

        float lamfact = MAX(0.0f, vec3_dot(&lvec, &info.normal));
        color = make_pixel(
            info.color.r * lamfact + info.color.r * scene->ambient,
            info.color.g * lamfact + info.color.g * scene->ambient,
            info.color.b * lamfact + info.color.b * scene->ambient
        );
    }
    else {
        color = scene->background;
    }
    return color;
}

void raytrace(struct Scene* scene, struct Image* img) {
    scene->camera->set_resolution(scene->camera, img->width, img->height);

    for(uint16_t j=0; j < img->height; j++) {
        for(uint16_t i=0; i < img->width; i++) {
            struct Ray ray = scene->camera->ij_ray(scene->camera, i, j);
            struct Interval interval = make_interval(0.00, INFINITY);
            struct Pixel color = raycolor(scene, &ray, &interval);
            color = color.quantize(&color, 255);
            img->set_pixel(img, (struct Point2I16){i, j}, color);
        }
    }
}
