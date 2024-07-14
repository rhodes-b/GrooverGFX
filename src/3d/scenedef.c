
#include "scenedef.h"

static struct Scene scene;
static struct Camera camera;

struct Scene* get_scene() {
    return &scene;
}

struct Camera* get_camera() {
    return &camera;
}

static void scene_add(struct Scene* s, struct Shape* obj) {
    s->objects.add(&s->objects, obj);
}

void init_scene() {
    camera = make_camera();
    scene = (struct Scene){
        .camera = &camera,
        .background = {0},
        .objects = make_group(),
        .add = scene_add,
    };
}
