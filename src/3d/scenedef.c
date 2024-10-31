
#include "scenedef.h"
#include "materials.h"

static struct Scene scene;
static struct Camera camera;
uint8_t n_lights = 0;

struct Scene* get_scene() {
    return &scene;
}

struct Camera* get_camera() {
    return &camera;
}

static void scene_add(struct Scene* s, struct Shape* obj) {
    s->objects.add(&s->objects, obj);
}

static void scene_add_light(struct Scene* s, struct Light light) {
    if(n_lights == MAX_LIGHTS) {
        return;
    }
    s->lights[n_lights] = light;
    n_lights++;
}

static void scene_set_light(struct Scene* s, uint8_t index, struct Light light) {
    if(index >= n_lights) {
        return;
    }
    s->lights[index] = light;
}

void init_scene() {
    init_materials();
    camera = make_camera();
    scene = (struct Scene){
        .camera = &camera,
        .objects = make_group(), 
        .background = make_pixel(1, 1, 1),
        .ambient = make_pixel(0.2, 0.2, 0.2),
        .reflections = 0,
        .shadows = false,
        .lights = {0},
        .add = scene_add,
        .add_light = scene_add_light,
        .set_light = scene_set_light,
    };
    scene.lights[0] = (struct Light){camera.eye, make_pixel(1.0, 1.0, 1.0)};
}

void free_scene() {
    for(uint16_t i=0; i < scene.objects.n_objects; i++) {
        struct Shape s = scene.objects.objects[i];
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

