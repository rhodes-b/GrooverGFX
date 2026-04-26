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
    cam.set_perspective(60, 1.3333, 5);
    var s = models.Shape{ .sphere = try models.Sphere.init(
        &alloc,
        .{ .vals = .{ 0, 0, -10 } },
        2,
        materials.make_material(gfx_types.Pixel{ .r = 0, .g = 1, .b = 0 }),
        20,
        20,
    ) };

    try scene.add(&alloc, &s);

    var io_threaded: std.Io.Threaded = .init(alloc, .{});
    const io = io_threaded.io();

    const cwd = try std.Io.Dir.cwd().realPathFileAlloc(io, ".", alloc);
    defer alloc.free(cwd);
    const img_path = try std.fs.path.join(alloc, &.{ cwd, "ppm/scene0-rt.ppm" });
    defer alloc.free(img_path);

    var img = try image.Image.init(alloc, 640, 480);
    defer img.deinit(alloc);
    render_ray.raytrace(scene, &img);
    try img.save(io, img_path);
}
