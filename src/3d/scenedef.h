
#ifndef _GROOVER_SCENEDEF_H
#define _GROOVER_SCENEDEF_H

#include "models.h"
#include "camera.h"

#define MAX_LIGHTS 100
extern uint8_t scene_n_lights;

struct Light {
    struct Point3F32 pos;
    struct Pixel color;
};

struct Scene {
    struct Camera* camera;
    struct Group objects;
    struct Pixel background;
    struct Pixel ambient;
    uint8_t reflections;
    bool shadows;
    struct Light lights[MAX_LIGHTS];
    
    void (*add)(struct Scene* s, struct Shape* obj);
    void (*add_light)(struct Scene* s, struct Light light);
    void (*set_light)(struct Scene* s, uint8_t index, struct Light light);
};

void init_scene();

void free_scene();

struct Scene* get_scene();
struct Camera* get_camera();

#endif /* _GROOVER_SCENEDEF_H */

