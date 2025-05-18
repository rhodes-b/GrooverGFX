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

    var cam = scenedef.get_camera();
    const scene = scenedef.get_scene();
    cam.set_perspective(60, 1.3333, 5);

    var b = models.Shape{
        .box = models.Box.init(.{ .vals = .{ -3, -2, -10 } }, .{ .vals = .{ 2, 2, 2 } }, .{ .r = 1, .g = 0, .b = 0 }),
    };

    try scene.add(&b);

    var img = try image.Image.init(alloc, 640, 480);
    try raytrace.raytrace(alloc, scene, &img);
    try img.save("/home/brice/workspace/GrooverGFX/ppm/scene2-rt.ppm");
}
