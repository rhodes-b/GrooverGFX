
#include <render_oo.h>
#include <scenedef.h>
#include <models.h>
#include <stdlib.h>
#include <time.h>
#include <render_ray.h>

float randf() {
    return (float) rand() / (uint64_t)RAND_MAX;
}

int main() {
    init_scene();
    srand(time(0));
    struct Camera* cam = get_camera();
    struct Scene* scene = get_scene();
    cam->set_perspective(cam, 60, 4./3, 50);

    struct Sphere s1 = make_sphere((struct Point3F32){0., 300., -1200.}, 200, make_pixel(1, 0, 0), 7, 15);
    struct Sphere s2 = make_sphere((struct Point3F32){-80., 150., -1200.}, 200, make_pixel(0, 1, 0), 7, 15);
    struct Sphere s3 = make_sphere((struct Point3F32){70., 100., -1200.}, 200, make_pixel(0, 0, 1), 7, 15);

    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s1});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s2});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s3});

    for(int8_t x=-2; x < 3; x++) {
        for(int8_t z=2; z < 8; z++) {
            struct Sphere s = make_sphere(
                (struct Point3F32){x*200, -300, z * -400},
                40,
                make_pixel(randf(), randf(), randf()),
                7,
                15
            );
            scene->add(scene, &(struct Shape){SPHERE, .shape.s = s});
        }
    }

    struct Image img_wf = make_image(640, 480);
    render_wireframe(scene, &img_wf);
    img_wf.save(&img_wf, "ppm/scene1-wf.ppm");
    free_image(&img_wf);

    struct Image img_rt = make_image(640, 480);
    raytrace(scene, &img_rt);
    img_rt.save(&img_rt, "ppm/scene1-rt.ppm");
    free_image(&img_rt);

    free_scene();

    return 0;

}