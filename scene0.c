
#include <render_oo.h>
#include <scenedef.h>
#include <models.h>


int main() {
    init_scene();
    struct Camera* cam = get_camera();
    struct Scene* scene = get_scene();
    cam->set_perspective(cam, 60, 1.3333, 5);

    struct Sphere s = make_sphere((struct Point3F32){0., 0., -10.}, 2, (struct Pixel){0, 255, 0}, 7, 15);

    struct Shape obj = {
        .shape_type = SPHERE,
        .shape.s = s,
    };

    scene->add(scene, &obj);

    struct Image img = make_image(640, 480);

    render_wireframe(scene, &img);

    img.save(&img, "ppm/scene0.ppm");

    return 0;

}