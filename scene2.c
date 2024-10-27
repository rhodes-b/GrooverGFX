
#include <render_oo.h>
#include <scenedef.h>
#include <models.h>
#include <render_ray.h>

// scene.background = (.8, .8, .8)
// TODO: make scene.background work (I don't have image->clear)
int main() {
    init_scene();
    struct Camera* cam = get_camera();
    struct Scene* scene = get_scene();
    cam->set_perspective(cam, 60, 1.3333, 5);

    struct Box b = make_box((struct Point3F32){-3, -2, -10}, (struct Vec3){2, 2, 2}, make_pixel(1, 0, 0));

    scene->add(scene, &(struct Shape){BOX, .shape.b = b});

    struct Image img_wf = make_image(640, 480);
    render_wireframe(scene, &img_wf);
    img_wf.save(&img_wf, "ppm/scene2-wf.ppm");

    struct Image img_sig = make_image(640, 480);
    render_signature(scene, &img_sig);
    img_sig.save(&img_sig, "ppm/scene2-sig.ppm");
    free_image(&img_sig);

    struct Image img_rt = make_image(640, 480);
    raytrace(scene, &img_rt);
    img_rt.save(&img_rt, "ppm/scene2-rt.ppm");

    return 0;
}
