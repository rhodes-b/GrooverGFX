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
    cam.set_perspective(30, 1.3333, 5);
    scene.background = .{ .r = 0.8, .g = 0.8, .b = 0.7 };

    var prng = std.Random.DefaultPrng.init(blk: {
        var seed: u64 = undefined;
        try std.posix.getrandom(std.mem.asBytes(&seed));
        break :blk seed;
    });

    var b1 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ -3, -2, -20 } }, .{ .vals = .{ 2, 2, 2 } }, materials.make_material(gfx_types.Pixel{ .r = 0.8, .g = 0, .b = 0 })),
    };
    var s1 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 2.5, -2, -20 } }, 1, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 0.8, .b = 0 }), 20, 20),
    };
    const rand_r = prng.random().float(f32);
    const rand_g = prng.random().float(f32);
    const rand_b = prng.random().float(f32);
    var s2 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 0, 0, -25 } }, 3, materials.make_material(gfx_types.Pixel{ .r = rand_r, .g = rand_g, .b = rand_b }), 20, 20),
    };
    var b2 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ 0, -3.5, -20 } }, .{ .vals = .{ 18, 0.5, 30 } }, materials.make_material(gfx_types.Pixel{ .r = 0.9, .g = 0.8, .b = 0.3 })),
    };

    try scene.add(&b1);
    try scene.add(&s1);
    try scene.add(&s2);
    try scene.add(&b2);

    const cwd = try std.fs.cwd().realpathAlloc(alloc, ".");
    defer alloc.free(cwd);
    const img_path = try std.fs.path.join(alloc, &.{ cwd, "ppm/scene3-rt.ppm" });
    defer alloc.free(img_path);

    var img = try image.Image.init(alloc, 640, 480);
    defer img.deinit(alloc);
    render_ray.raytrace(scene, &img);
    try img.save(img_path);
}
