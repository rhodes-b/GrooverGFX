const std = @import("std");
const scenedef = @import("scenedef.zig");
const image = @import("image.zig");
const ray3d = @import("ray3d.zig");
const gfx_types = @import("gfx_types.zig");
const models = @import("models.zig");

fn raycolor(alloc: std.mem.Allocator, scene: *scenedef.Scene, r: *ray3d.Ray, interval: *ray3d.Interval) !gfx_types.Pixel {
    var info: models.Record = undefined;
    // @memset(&info, 0);
    var color = gfx_types.Pixel{ .r = 0, .g = 0, .b = 0 };

    const hit = try scene.objects.intersect(alloc, r, interval, &info);
    if (hit) {
        const tmp = r.point_at(info.time);
        const lvec = scene.camera.eye.sub(&tmp).normalized();
        const lambfact = @max(0.0, lvec.dot(&info.normal));
        color = gfx_types.Pixel{
            .r = info.color.r * lambfact + info.color.r * scene.ambient,
            .g = info.color.g * lambfact + info.color.g * scene.ambient,
            .b = info.color.b * lambfact + info.color.b * scene.ambient,
        };

        // this solves the if below?
        alloc.free(info.points);
    } else {
        color = scene.background;
    }
    // if (info.pts != NULL) {
    //     free(info.pts)
    // }
    return color;
}

pub fn raytrace(alloc: std.mem.Allocator, scene: *scenedef.Scene, img: *image.Image) !void {
    scene.camera.set_resolution(img.width, img.height);

    for (0..img.height) |j| {
        for (0..img.width) |i| {
            var ray = scene.camera.ij_ray(@floatFromInt(i), @floatFromInt(j));
            var interval = ray3d.Interval{ .low = 0.00, .high = std.math.inf(f32) };
            var color = try raycolor(alloc, scene, &ray, &interval);
            color = color.quantize(255);
            img.set_pixel(.{ .vals = .{ @intCast(i), @intCast(j) } }, color);
        }
    }
}
