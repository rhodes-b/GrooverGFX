
#ifndef _GROOVER_MODELS_H
#define _GROOVER_MODELS_H

#include "gfx_types.h"
#include "math3d.h"

struct Sphere {
    struct Point3F32 pos;
    float radius;
    struct Pixel color;
    uint8_t nlat;
    uint8_t nlong;
    struct Vec3 axis;
    struct Point3F32 northpole;
    struct Point3F32 southpole;

    // TODO: return type or pointer to get vals?
    void (*iter_polygons)(struct Sphere* s);
    // TODO: figure out info....
    bool (*intersect)(struct Sphere* s, struct Ray* r, struct Interval* i, void* info);
};

struct Box {
    struct Point3F32 pos;
    struct Vec3 size;
    struct Pixel color;
    struct Point3F32 planes[3]; // TODO: is this correct?

    // TODO: return type or pointer to get vals?
    void (*iter_polygons)(struct Box* b);
    // TODO: figure out info....
    bool (*intersect)(struct Box* b, struct Ray* r, struct Interval* i, void* info);
};

enum ShapeType {
    SPHERE,
    BOX,
};

struct Shape {
    union ShapeVal {
        struct Box b;
        struct Sphere s;
    } shape;
    enum ShapeType shape_type;
};

struct Group {
    struct Shape objects[100];

    // TODO: what is a model?
    void (*add)(struct Group* g, void* model);
    // TODO: return type or pointer to get vals?
    void (*iter_polygons)(struct Group* g);
    // TODO: figure out info....
    bool (*intersect)(struct Group* g, struct Ray* r, struct Interval* i, void* info);
};

struct Sphere make_sphere(struct Point3F23 pos, float radius, struct Pixel color, uint8_t nlat, uint8_t nlong);

struct Box make_box(struct Point3F32 pos, struct Vec3 size, struct Pixel color);

/*

// TODO: this might be impossible in c?
// its just a dictionary that holds anything...
# ----------------------------------------------------------------------
class Record:

    def __init__(self, **items):
        self.__dict__.update(items)

    def update(self, **items):
        self.__dict__.update(**items)

    def __repr__(self):
        d = self.__dict__
        fields = [k+"="+str(d[k]) for k in sorted(d)]
        return "Record({})".format(", ".join(fields))
*/

#endif /* _GROOVER_MODELS_H */