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

    var s = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 0, 0, -10 } }, 2, .{ .r = 0, .g = 1, .b = 1 }, 20, 20),
    };

    try scene.add(&s);

    var img = try image.Image.init(alloc, 640, 480);
    try raytrace.raytrace(alloc, scene, &img);
    try img.save("/home/brice/workspace/GrooverGFX/ppm/scene0-rt.ppm");

    // free_image(&img_rt);

}
