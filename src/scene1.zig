const std = @import("std");
const scenedef = @import("3d/scenedef.zig");
const render_ray = @import("3d/render_ray.zig");
const models = @import("3d/models.zig");
const image = @import("3d/image.zig");
const raytrace = @import("3d/render_ray.zig");

pub fn main() !void {
    var debug_alloc = std.heap.DebugAllocator(.{}).init;
    const alloc = debug_alloc.allocator();

    try scenedef.init_scene(alloc);
    defer scenedef.deinit_scene(alloc);

    var cam = scenedef.get_camera();
    const scene = scenedef.get_scene();
    scene.light = .{ .vals = .{ 0, 550, -1200 } };
    cam.set_perspective(60, 4.0 / 3.0, 5);

    var s1 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 0, 300, -1200 } }, 200, .{ .r = 1, .g = 0, .b = 0 }, 20, 20),
    };
    var s2 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ -80, 150, -1200 } }, 200, .{ .r = 0, .g = 1, .b = 0 }, 20, 20),
    };
    var s3 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 70, 100, -1200 } }, 200, .{ .r = 0, .g = 0, .b = 1 }, 20, 20),
    };

    try scene.add(&s1);
    try scene.add(&s2);
    try scene.add(&s3);

    var prng = std.Random.DefaultPrng.init(blk: {
        var seed: u64 = undefined;
        try std.posix.getrandom(std.mem.asBytes(&seed));
        break :blk seed;
    });

    const vals = [_]isize{ -2, -1, 0, 1, 2 };
    var s: models.Shape = undefined;
    for (vals) |x| {
        for (2..8) |z| {
            const x_f32 = @as(f32, @floatFromInt(x)) * 200.0;
            const z_f32 = @as(f32, @floatFromInt(z)) * -400.0;
            const r = prng.random().float(f32);
            const g = prng.random().float(f32);
            const b = prng.random().float(f32);
            s = models.Shape{
                .sphere = try models.Sphere.init(alloc, .{ .vals = .{ x_f32, -300, z_f32 } }, 40, .{ .r = r, .g = g, .b = b }, 20, 20),
            };
            try scene.add(&s);
        }
    }
    const cwd = try std.fs.cwd().realpathAlloc(alloc, ".");
    defer alloc.free(cwd);
    const img_path = try std.fs.path.join(alloc, &.{ cwd, "ppm/scene1-rt.ppm" });
    defer alloc.free(img_path);

    var img = try image.Image.init(alloc, 640, 480);
    defer img.deinit(alloc);
    try raytrace.raytrace(alloc, scene, &img);
    try img.save(img_path);
}
