const std = @import("std");
const scenedef = @import("scenedef.zig");
const image = @import("image.zig");
const ray3d = @import("ray3d.zig");
const gfx_types = @import("gfx_types.zig");
const models = @import("models.zig");

fn raycolor(scene: *scenedef.Scene, r: *const ray3d.Ray, interval: *ray3d.Interval, nreflect: u8) gfx_types.Pixel {
    var info: models.Record = undefined;
    var color = gfx_types.Pixel{ .r = 0, .g = 0, .b = 0 };

    const hit = scene.objects.intersect(r, interval, &info);
    if (hit) {
        const k = info.color;
        const hitpt = info.point;
        const n = info.normal;

        color = k.ambient.mul(scene.ambient);

        for (scene.lights.items) |light| {
            var lpos: gfx_types.Point(f32, 3) = undefined;
            var lcolor: gfx_types.Pixel = undefined;
            lpos, lcolor = light;

            var lvec = lpos.sub(&hitpt);

            var shadow_interval = ray3d.Interval{ .low = 0.0001, .high = 1 };
            var shadow_rec: models.Record = undefined;
            if (!(scene.shadows and scene.objects.intersect(&.{ .start = hitpt, .dir = lvec }, &shadow_interval, &shadow_rec))) {
                lvec = lvec.normalized();
                const lambert = @max(0.0, lvec.dot(&n));
                const diffuse = k.diffuse.mul(lambert);
                color = color.add(&diffuse);

                var new_color = color.add(&k.specular.mul(lcolor));

                const vvec = r.dir.normalized().neg();
                const h = lvec.add(&vvec).normalized();
                const shine = std.math.pow(f32, @max(0.0, h.dot(&n)), k.shininess);

                new_color = new_color.mul(shine);
                color = color.add(&new_color);
            }
        }
        if (k.reflect != null and nreflect > 0) {
            var ray_interval = ray3d.Interval{ .low = 0.0001, .high = std.math.inf(f32) };
            var rcolor = raycolor(scene, &.{ .start = hitpt, .dir = r.dir.reflect(&n) }, &ray_interval, nreflect - 1);
            rcolor = rcolor.mul(k.reflect.?);
            color = color.add(&rcolor);
        }
    } else {
        color = scene.background;
    }

    return color;
}

pub fn raytrace(scene: *scenedef.Scene, img: *image.Image) void {
    scene.camera.set_resolution(img.width, img.height);

    for (0..img.height) |j| {
        for (0..img.width) |i| {
            var ray = scene.camera.ij_ray(@floatFromInt(i), @floatFromInt(j));
            var interval = ray3d.Interval{ .low = 0.00, .high = std.math.inf(f32) };
            var color = raycolor(scene, &ray, &interval, scene.reflections);
            color = color.quantize(255);
            img.set_pixel(.{ .vals = .{ @intCast(i), @intCast(j) } }, color);
        }
    }
}
