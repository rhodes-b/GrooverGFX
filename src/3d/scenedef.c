
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
        .background = make_pixel(1, 1, 1),
        .objects = make_group(),
        .add = scene_add,
        .ambient = 0.2,
    };
}

void free_scene() {
    for(uint16_t i=0; i < scene.objects.n_objects; i++) {
        struct Shape s = scene.objects.objects[0];
        switch(s.shape_type) {
            case SPHERE:
                free_sphere(&s.shape.s);
                break;
            case BOX:
                // don't need to free memory here
                break;
            default:
                // unreachable
                break;
        }
    }
}
