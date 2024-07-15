
#ifndef _GROOVER_MODELS_H
#define _GROOVER_MODELS_H

#include <stdbool.h>
#include "gfx_types.h"
#include "math3d.h"
#include "ray3d.h"
#include "linked_list.h"

struct Sphere {
    struct Point3F32 pos;
    float radius;
    struct Pixel color;
    uint8_t nlat;
    uint8_t nlong;
    struct Point3F32* bands;
    struct Vec3 axis;
    struct Point3F32 northpole;
    struct Point3F32 southpole;

    struct Node* (*iter_polygons)(struct Sphere* s);
    bool (*intersect)(struct Sphere* s, struct Ray* r, struct Interval* i, struct Record* info);
};

struct Box {
    struct Point3F32 pos;
    struct Vec3 size;
    struct Pixel color;
    struct Point2F32 planes[3];
   
    struct Node* (*iter_polygons)(struct Box* b);
    bool (*intersect)(struct Box* b, struct Ray* r, struct Interval* i, struct Record* info);
};

enum ShapeType {
    SPHERE,
    BOX,
};

struct Shape {
    enum ShapeType shape_type;
    union ShapeVal {
        struct Box b;
        struct Sphere s;
    } shape;
};


struct Group {
    struct Shape objects[100];
    uint8_t n_objects;

    void (*add)(struct Group* g, struct Shape* model);
    struct Node* (*iter_polygons)(struct Group* g);
    bool (*intersect)(struct Group* g, struct Ray* r, struct Interval* i, struct Record* info);
};

void free_record(struct Record* r);

struct Sphere make_sphere(struct Point3F32 pos, float radius, struct Pixel color, uint8_t nlat, uint8_t nlong);

void free_sphere(struct Sphere* s);

struct Box make_box(struct Point3F32 pos, struct Vec3 size, struct Pixel color);

struct Group make_group();

#endif /* _GROOVER_MODELS_H */