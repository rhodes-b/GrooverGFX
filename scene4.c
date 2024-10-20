
#include <render_oo.h>
#include <scenedef.h>
#include <models.h>
#include <render_ray.h>

int main() {
    init_scene();
    struct Camera* cam = get_camera();
    struct Scene* scene = get_scene();
    cam->set_perspective(cam, 120, 4./3, 20);

    // head
    struct Sphere s1 = make_sphere((struct Point3F32){0, 15, -30}, 5, make_pixel(0, 1, 1), 20, 20);
    struct Sphere s2 = make_sphere((struct Point3F32){-2, 15, -26}, 1, make_pixel(0, 0, 0), 20, 20);
    struct Sphere s3 = make_sphere((struct Point3F32){2, 15, -26}, 1, make_pixel(0, 0, 0), 20, 20);

    // arms
    struct Box b1 = make_box((struct Point3F32){-10, 10, -30}, (struct Vec3){20, 2, 3}, make_pixel(0, 1, 1));
    struct Box b2 = make_box((struct Point3F32){10, 10, -30}, (struct Vec3){20, 2, 3}, make_pixel(0, 1, 1));
    struct Sphere s4 = make_sphere((struct Point3F32){-20, 10, -30}, 2, make_pixel(0, 1, 1), 20, 20);
    struct Sphere s5 = make_sphere((struct Point3F32){20, 10, -30}, 2, make_pixel(0, 1, 1), 20, 20);

    // torso
    struct Box b3 = make_box((struct Point3F32){0, 0, -30}, (struct Vec3){10, 20, 5}, make_pixel(0, 1, 1));

    // legs
    struct Sphere s6 = make_sphere((struct Point3F32){-6, -10, -30}, 2, make_pixel(0, 1, 1), 20, 20);
    struct Sphere s7 = make_sphere((struct Point3F32){6, -10, -30}, 2, make_pixel(0, 1, 1), 20, 20);
    struct Box b4 = make_box((struct Point3F32){-6, -20, -30}, (struct Vec3){2, 20, 3}, make_pixel(0, 1, 1));
    struct Box b5 = make_box((struct Point3F32){6, -20, -30}, (struct Vec3){2, 20, 3}, make_pixel(0, 1, 1));
    struct Sphere s8 = make_sphere((struct Point3F32){-6, -30, -28}, 2, make_pixel(0, 1, 1), 20, 20);
    struct Sphere s9 = make_sphere((struct Point3F32){6, -30, -28}, 2, make_pixel(0, 1, 1), 20, 20);


    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s1});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s2});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s3});
    
    scene->add(scene, &(struct Shape){BOX, .shape.b = b1});
    scene->add(scene, &(struct Shape){BOX, .shape.b = b2});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s4});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s5});

    scene->add(scene, &(struct Shape){BOX, .shape.b = b3});

    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s6});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s7});
    scene->add(scene, &(struct Shape){BOX, .shape.b = b4});
    scene->add(scene, &(struct Shape){BOX, .shape.b = b5});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s8});
    scene->add(scene, &(struct Shape){SPHERE, .shape.s = s9});

    struct Image img_wf = make_image(640, 480);
    render_wireframe(scene, &img_wf);
    img_wf.save(&img_wf, "ppm/scene4-wf.ppm");

    struct Image img_sig = make_image(640, 480);
    render_signature(scene, &img_sig);
    img_sig.save(&img_sig, "ppm/scene4-sig.ppm");
    free_image(&img_sig);

    struct Image img_rt = make_image(640, 480);
    raytrace(scene, &img_rt);
    img_rt.save(&img_rt, "ppm/scene4-rt.ppm");

    free_scene();

    return 0;
}
