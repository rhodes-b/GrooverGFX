
#include <render_oo.h>
#include <scenedef.h>
#include <models.h>
#include <stdlib.h>
#include <time.h>


// scene.background = (.8, .8, .7)
// TODO: make scene.background work (I don't have image->clear)
int main() {
    init_scene();
    srand(time(0));
    struct Camera* cam = get_camera();
    struct Scene* scene = get_scene();
    cam->set_perspective(cam, 30, 1.3333, 5);

    struct Box b1 = make_box((struct Point3F32){-3, -2, -20}, (struct Vec3){2, 2, 2}, (struct Pixel){204, 0, 0});
    struct Sphere s1 = make_sphere((struct Point3F32){2.5, -2, -20}, 1, (struct Pixel){0, 204, 0}, 7, 15);
    struct Sphere s2 = make_sphere((struct Point3F32){0, 0, -25}, 3, (struct Pixel){rand()%255, rand()%255, rand()%255}, 7, 15);
    struct Box b2 = make_box((struct Point3F32){0, -3.5, -20}, (struct Vec3){18, .5, 30}, (struct Pixel){229, 204, 76});

    scene->add(scene, &(struct Shape){BOX, .shape.b = b1});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s1});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s2});
    scene->add(scene, &(struct Shape){BOX, .shape.b = b2});

    struct Image img_wf = make_image(640, 480);

    render_wireframe(scene, &img_wf);

    img_wf.save(&img_wf, "ppm/scene3-wf.ppm");

    return 0;
}
