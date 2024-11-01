
#ifndef _GROOVER_SCENEDEF_H
#define _GROOVER_SCENEDEF_H

#include "models.h"
#include "camera.h"

struct Scene {
    struct Camera* camera;
    struct Group objects;
    struct Pixel background;
    float ambient;
    struct Point3F32 light;

    void (*add)(struct Scene* s, struct Shape* obj);

};

void init_scene();

void free_scene();

struct Scene* get_scene();
struct Camera* get_camera();

#endif /* _GROOVER_SCENEDEF_H */
