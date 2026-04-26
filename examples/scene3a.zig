const std = @import("std");

const groove_gfx = @import("groove_gfx");
const gfx_types = groove_gfx.gfx_types;
const image = groove_gfx.image;
const materials = groove_gfx.materials;
const models = groove_gfx.models;
const render_ray = groove_gfx.render_ray;
const scenedef = groove_gfx.scenedef;

pub fn main() !void {
    var debug_alloc: std.heap.DebugAllocator(.{}) = .init;
    var alloc = debug_alloc.allocator();

    try scenedef.init_scene(&alloc);
    defer scenedef.deinit_scene(&alloc);

    var cam = scenedef.get_camera();
    const scene = scenedef.get_scene();
    cam.set_perspective(30, 1.3333, 5);
    scene.background = .{ .r = 0.5, .g = 0.5, .b = 1 };
    // scene.ambient = (.3, .3, .3)
    scene.ambient = 0.3;

    var b1 = models.Shape{ .box = models.Box.init(
        .{ .vals = .{ -3, -2, -20 } },
        .{ .vals = .{ 2, 2, 2 } },
        materials.RED_PLASTIC,
    ) };
    var s1 = models.Shape{ .sphere = try models.Sphere.init(
        &alloc,
        .{ .vals = .{ 2.5, -2, -20 } },
        1,
        materials.SILVER,
        20,
        20,
    ) };
    var s2 = models.Shape{ .sphere = try models.Sphere.init(
        &alloc,
        .{ .vals = .{ 0, 0, -25 } },
        3,
        materials.BRASS,
        20,
        20,
    ) };

    var b2 = models.Shape{ .box = models.Box.init(
        .{ .vals = .{ 0, -3.5, -20 } },
        .{ .vals = .{ 18, 1, 30 } },
        materials.make_material(gfx_types.Pixel{ .r = 0.9, .g = 0.8, .b = 0.3 }),
    ) };

    try scene.add(&alloc, &b1);
    try scene.add(&alloc, &s1);
    try scene.add(&alloc, &s2);
    try scene.add(&alloc, &b2);


    var io_threaded: std.Io.Threaded = .init(alloc, .{});
    const io = io_threaded.io();

    const cwd = try std.Io.Dir.cwd().realPathFileAlloc(io, ".", alloc);
    defer alloc.free(cwd);
    const img_path = try std.fs.path.join(alloc, &.{ cwd, "ppm/scene3a-rt.ppm" });
    defer alloc.free(img_path);

    var img = try image.Image.init(alloc, 640, 480);
    defer img.deinit(alloc);
    render_ray.raytrace(scene, &img);
    try img.save(io, img_path);
}
