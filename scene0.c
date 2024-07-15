
#include <render_oo.h>
#include <scenedef.h>
#include <models.h>
#include <render_ray.h>

int main() {
    init_scene();
    struct Camera* cam = get_camera();
    struct Scene* scene = get_scene();
    cam->set_perspective(cam, 60, 1.3333, 5);

    struct Sphere s = make_sphere((struct Point3F32){0., 0., -10.}, 2, make_pixel(0, 1, 0), 7, 15);

    struct Shape obj = {
        .shape_type = SPHERE,
        .shape.s = s,
    };

    scene->add(scene, &obj);

    struct Image img_wf = make_image(640, 480);
    render_wireframe(scene, &img_wf);
    img_wf.save(&img_wf, "ppm/scene0-wf.ppm");
    free_image(&img_wf);

    struct Image img_rt = make_image(640, 480);
    raytrace(scene, &img_rt);
    img_rt.save(&img_rt, "ppm/scene0-rt.ppm");
    free_image(&img_rt);

    free_scene();

    return 0;

}