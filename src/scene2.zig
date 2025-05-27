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

    var b = models.Shape{
        .box = models.Box.init(.{ .vals = .{ -3, -2, -10 } }, .{ .vals = .{ 2, 2, 2 } }, materials.make_material(gfx_types.Pixel{ .r = 1, .g = 0, .b = 0 })),
    };

    try scene.add(&b);

    const cwd = try std.fs.cwd().realpathAlloc(alloc, ".");
    defer alloc.free(cwd);
    const img_path = try std.fs.path.join(alloc, &.{ cwd, "ppm/scene2-rt.ppm" });
    defer alloc.free(img_path);

    var img = try image.Image.init(alloc, 640, 480);
    defer img.deinit(alloc);
    raytrace.raytrace(scene, &img);
    try img.save(img_path);
}
