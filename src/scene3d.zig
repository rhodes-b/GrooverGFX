const std = @import("std");
const scenedef = @import("3d/scenedef.zig");
const render_ray = @import("3d/render_ray.zig");
const models = @import("3d/models.zig");
const materials = @import("3d/materials.zig");
const gfx_types = @import("3d/gfx_types.zig");
const image = @import("3d/image.zig");
const raytrace = @import("3d/render_ray.zig");

pub fn main() !void {
    var debug_alloc = std.heap.DebugAllocator(.{}).init;
    const alloc = debug_alloc.allocator();

    try scenedef.init_scene(alloc);
    defer scenedef.deinit_scene(alloc);

    var cam = scenedef.get_camera();
    const scene = scenedef.get_scene();
    cam.set_perspective(30, 1.3333, 5);
    scene.background = .{ .r = 0.5, .g = 0.5, .b = 1 };
    // scene.ambient = (.3, .3, .3)
    scene.ambient = 0.3;
    scene.lights.items[0] = .{ .{ .vals = .{ -150, 200, 50 } }, .{ .r = 0.5, .g = 0.5, .b = 0.5 } };
    try scene.lights.append(.{ .{ .vals = .{ 150, 50, 50 } }, .{ .r = 0.3, .g = 0.3, .b = 0.3 } });

    scene.shadows = true;

    var b1 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ -3, -2, -20 } }, .{ .vals = .{ 2, 2, 2 } }, materials.RED_PLASTIC),
    };
    var s1 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 2.5, -2, -20 } }, 1, materials.SILVER, 20, 20),
    };
    var s2 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 0, 0, -25 } }, 3, materials.BRASS, 20, 20),
    };

    var b2 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ 0, -3.5, -20 } }, .{ .vals = .{ 18, 1, 30 } }, materials.make_material(gfx_types.Pixel{ .r = 0.45, .g = 0.4, .b = 0.15 })),
    };

    try scene.add(&b1);
    try scene.add(&s1);
    try scene.add(&s2);
    try scene.add(&b2);

    const cwd = try std.fs.cwd().realpathAlloc(alloc, ".");
    defer alloc.free(cwd);
    const img_path = try std.fs.path.join(alloc, &.{ cwd, "ppm/scene3d-rt.ppm" });
    defer alloc.free(img_path);

    var img = try image.Image.init(alloc, 640, 480);
    defer img.deinit(alloc);
    raytrace.raytrace(scene, &img);
    try img.save(img_path);
}
