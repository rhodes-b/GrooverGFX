const std = @import("std");

const groove_gfx = @import("groove_gfx");
const gfx_types = groove_gfx.gfx_types;
const image = groove_gfx.image;
const materials = groove_gfx.materials;
const models = groove_gfx.models;
const render_ray = groove_gfx.render_ray;
const scenedef = groove_gfx.scenedef;

pub fn main() !void {
    var debug_alloc = std.heap.DebugAllocator(.{}).init;
    const alloc = debug_alloc.allocator();

    try scenedef.init_scene(alloc);
    defer scenedef.deinit_scene(alloc);

    var cam = scenedef.get_camera();
    const scene = scenedef.get_scene();
    scene.lights.items[0] = .{ .{ .vals = .{ 0, 550, -1200 } }, gfx_types.Pixel{ .r = 1, .g = 1, .b = 1 } };
    cam.set_perspective(60, 4.0 / 3.0, 5);

    var s1 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 0, 300, -1200 } }, 200, materials.make_material(gfx_types.Pixel{ .r = 1, .g = 0, .b = 0 }), 20, 20),
    };
    var s2 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ -80, 150, -1200 } }, 200, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 0 }), 20, 20),
    };
    var s3 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 70, 100, -1200 } }, 200, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 0, .b = 1 }), 20, 20),
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
                .sphere = try models.Sphere.init(alloc, .{ .vals = .{ x_f32, -300, z_f32 } }, 40, materials.make_material(gfx_types.Pixel{ .r = r, .g = g, .b = b }), 20, 20),
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
    render_ray.raytrace(scene, &img);
    try img.save(img_path);
}
