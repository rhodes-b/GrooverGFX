/*

# render_ray.py
#    Ray tracing rendering algorithms

from ren3d.ray3d import Interval
from ren3d.models import Record


def raytrace(scene, img, updatefn=None):
    """basic raytracing algorithm to render scene into img"""
    img.clear(scene.background.quantize(255))
    w, h = img.size
    scene.camera.set_resolution(w, h)

    for j in range(h):
        for i in range(w):
            ray = scene.camera.ij_ray(i, j)
            color = raycolor(scene, ray, Interval())
            img[(i, j)] = color.quantize(255)
        if updatefn:
            updatefn()


def raycolor(scene, ray, interval):
    """returns the color of ray in the scene"""
    info = Record()
    if scene.objects.intersect(ray, interval, info):
        lvec = (scene.camera.eye - ray.point_at(interval.high)).normalized()  # normalized since it returns
        norm = info.normal

        lamfact = max(0.0, lvec.dot(norm))
        color = info.color * lamfact + info.color * scene.ambient
    else:
        color = scene.background

    return color


*/