
#include <render_oo.h>
#include <scenedef.h>
#include <models.h>
#include <stdlib.h>
#include <time.h>


int main() {
    init_scene();
    srand(time(0));
    struct Camera* cam = get_camera();
    struct Scene* scene = get_scene();
    cam->set_perspective(cam, 60, 4./3, 50);

    struct Sphere s1 = make_sphere((struct Point3F32){0., 300., -1200.}, 200, (struct Pixel){255, 0, 0}, 7, 15);
    struct Sphere s2 = make_sphere((struct Point3F32){-80., 150., -1200.}, 200, (struct Pixel){0, 255, 0}, 7, 15);
    struct Sphere s3 = make_sphere((struct Point3F32){70., 100., -1200.}, 200, (struct Pixel){0, 0, 255}, 7, 15);

    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s1});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s2});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s3});

    for(int8_t x=-2; x < 3; x++) {
        for(int8_t z=2; z < 8; z++) {
            struct Sphere s = make_sphere(
                (struct Point3F32){x*200, -300, z * -400},
                40,
                (struct Pixel){rand()%255, rand()%255, rand()%255},
                7,
                15
            );
            scene->add(scene, &(struct Shape){SPHERE, .shape.s = s});
        }
    }

    struct Image img_wf = make_image(640, 480);

    render_wireframe(scene, &img_wf);

    img_wf.save(&img_wf, "ppm/scene1-wf.ppm");

    return 0;

}