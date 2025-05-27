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
    cam.set_perspective(60, 1.3333, 5);

    var s = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 0, 0, -10 } }, 2, materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 0 }), 20, 20),
    };

    try scene.add(&s);

    const cwd = try std.fs.cwd().realpathAlloc(alloc, ".");
    defer alloc.free(cwd);
    const img_path = try std.fs.path.join(alloc, &.{ cwd, "ppm/scene0-rt.ppm" });
    defer alloc.free(img_path);

    var img = try image.Image.init(alloc, 640, 480);
    defer img.deinit(alloc);
    raytrace.raytrace(scene, &img);
    try img.save(img_path);
}
