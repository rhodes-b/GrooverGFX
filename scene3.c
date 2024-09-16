
#include <render_oo.h>
#include <scenedef.h>
#include <models.h>
#include <stdlib.h>
#include <time.h>
#include <render_ray.h>

float randf() {
    return (float) rand() / (uint64_t)RAND_MAX;
}

// scene.background = (.8, .8, .7)
// TODO: make scene.background work (I don't have image->clear)
int main() {
    init_scene();
    srand(time(0));
    struct Camera* cam = get_camera();
    struct Scene* scene = get_scene();
    cam->set_perspective(cam, 30, 1.3333, 5);

    struct Box b1 = make_box((struct Point3F32){-3, -2, -20}, (struct Vec3){2, 2, 2}, make_pixel(.8, 0, 0));
    struct Sphere s1 = make_sphere((struct Point3F32){2.5, -2, -20}, 1, make_pixel(0, .8, 0), 7, 15);
    struct Sphere s2 = make_sphere((struct Point3F32){0, 0, -25}, 3, make_pixel(randf(), randf(), randf()), 7, 15);
    struct Box b2 = make_box((struct Point3F32){0, -3.5, -20}, (struct Vec3){18, .5, 30}, make_pixel(.9, .8, .3));

    scene->add(scene, &(struct Shape){BOX, .shape.b = b1});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s1});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s2});
    scene->add(scene, &(struct Shape){BOX, .shape.b = b2});

    struct Image img_wf = make_image(640, 480);
    render_wireframe(scene, &img_wf);
    img_wf.save(&img_wf, "ppm/scene3-wf.ppm");

    struct Image img_rt = make_image(640, 480);
    raytrace(scene, &img_rt);
    img_rt.save(&img_rt, "ppm/scene3-rt.ppm");

    return 0;
}
