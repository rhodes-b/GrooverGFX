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
    cam.set_perspective(120, 4.0 / 3.0, 20);

    // head
    var s1 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 0, 15, -30 } }, 5, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 }), 20, 20),
    };
    var s2 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ -2, 15, -26 } }, 1, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 0, .b = 0 }), 20, 20),
    };
    var s3 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 2, 15, -26 } }, 1, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 0, .b = 0 }), 20, 20),
    };

    // arms
    var b1 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ -10, 10, -30 } }, .{ .vals = .{ 20, 2, 3 } }, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 })),
    };
    var b2 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ 10, 10, -30 } }, .{ .vals = .{ 20, 2, 3 } }, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 })),
    };
    var s4 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ -20, 10, -30 } }, 2, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 }), 20, 20),
    };
    var s5 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 20, 10, -30 } }, 2, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 }), 20, 20),
    };

    // torso
    var b3 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ 0, 0, -30 } }, .{ .vals = .{ 10, 20, 5 } }, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 })),
    };

    // legs
    var s6 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ -6, -10, -30 } }, 2, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 }), 20, 20),
    };
    var s7 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 6, -10, -30 } }, 2, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 }), 20, 20),
    };
    var b4 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ -6, -20, -30 } }, .{ .vals = .{ 2, 20, 3 } }, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 })),
    };
    var b5 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ 6, -20, -30 } }, .{ .vals = .{ 2, 20, 3 } }, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 })),
    };
    var s8 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ -6, -30, -28 } }, 2, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 }), 20, 20),
    };
    var s9 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 6, -30, -28 } }, 2, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 1 }), 20, 20),
    };

    try scene.add(&s1);
    try scene.add(&s2);
    try scene.add(&s3);

    try scene.add(&b1);
    try scene.add(&b2);
    try scene.add(&s4);
    try scene.add(&s5);

    try scene.add(&b3);

    try scene.add(&s6);
    try scene.add(&s7);
    try scene.add(&b4);
    try scene.add(&b5);
    try scene.add(&s8);
    try scene.add(&s9);

    const cwd = try std.fs.cwd().realpathAlloc(alloc, ".");
    defer alloc.free(cwd);
    const img_path = try std.fs.path.join(alloc, &.{ cwd, "ppm/scene4-rt.ppm" });
    defer alloc.free(img_path);

    var img = try image.Image.init(alloc, 640, 480);
    defer img.deinit(alloc);
    raytrace.raytrace(scene, &img);
    try img.save(img_path);
}
