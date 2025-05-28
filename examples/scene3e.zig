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
    scene.background = .{ .r = 0.0, .g = 0.0, .b = 0 };
    // scene.ambient = (.3, .3, .3)
    scene.ambient = 0.1;
    scene.lights.items[0] = .{ .{ .vals = .{ -150, 200, 50 } }, .{ .r = 0.5, .g = 0.5, .b = 0.5 } };
    try scene.lights.append(.{ .{ .vals = .{ 150, 50, 50 } }, .{ .r = 0.3, .g = 0.3, .b = 0.3 } });

    scene.shadows = true;
    scene.reflections = 3;

    var b1 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ -3, -2, -20 } }, .{ .vals = .{ 2, 2, 2 } }, materials.RED_PLASTIC),
    };

    var rsilver = materials.SILVER;
    rsilver.reflect = .{ .r = 0.5, .g = 0.5, .b = 0.5 };
    rsilver.diffuse = rsilver.diffuse.mul(0.5);
    rsilver.ambient = rsilver.ambient.mul(0.5);

    var s1 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 2.5, -2, -20 } }, 1, rsilver, 20, 20),
    };

    var rbrass = materials.BRASS;
    rbrass.reflect = .{ .r = 0.2, .g = 0.2, .b = 0.2 };

    var s2 = models.Shape{
        .sphere = try models.Sphere.init(alloc, .{ .vals = .{ 0, 0, -25 } }, 3, rbrass, 20, 20),
    };

    const bmat = materials.Material{ .diffuse = .{ .r = 0.4, .g = 0.4, .b = 0.4 }, .ambient = .{ .r = 0.4, .g = 0.4, .b = 0.4 }, .specular = .{ .r = 0.5, .g = 0.5, .b = 0.5 }, .shininess = 30, .reflect = .{ .r = 0.4, .g = 0.4, .b = 0.4 } };
    var b2 = models.Shape{
        .box = models.Box.init(.{ .vals = .{ 0, -3.5, -20 } }, .{ .vals = .{ 18, 1, 30 } }, bmat),
    };

    try scene.add(&b1);
    try scene.add(&s1);
    try scene.add(&s2);
    try scene.add(&b2);

    const cwd = try std.fs.cwd().realpathAlloc(alloc, ".");
    defer alloc.free(cwd);
    const img_path = try std.fs.path.join(alloc, &.{ cwd, "ppm/scene3e-rt.ppm" });
    defer alloc.free(img_path);

    var img = try image.Image.init(alloc, 640, 480);
    defer img.deinit(alloc);
    render_ray.raytrace(scene, &img);
    try img.save(img_path);
}
