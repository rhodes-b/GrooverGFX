const std = @import("std");
const camera = @import("camera.zig");
const models = @import("models.zig");
const gfx_types = @import("gfx_types.zig");

pub const Scene = struct {
    const Self = @This();

    camera: *camera.Camera,
    objects: models.Group,
    background: gfx_types.Pixel,
    ambient: f32,
    shadows: bool,
    reflections: u8,
    lights: std.ArrayList(struct { gfx_types.Point(f32, 3), gfx_types.Pixel }),

    pub fn add(self: *Self, obj: *models.Shape) !void {
        try self.objects.add(obj.*);
    }
};

var scene: Scene = undefined;
var cam: camera.Camera = undefined;

pub fn init_scene(alloc: std.mem.Allocator) !void {
    cam = camera.Camera{};
    scene = Scene{
        .camera = &cam,
        .objects = try models.Group.init(alloc),
        .background = gfx_types.Pixel{ .r = 255, .g = 255, .b = 255 },
        .ambient = 0.2,
        .shadows = false,
        .reflections = 0,
        .lights = @FieldType(Scene, "lights").init(alloc),
    };
    try scene.lights.append(.{ cam.eye, .{ .r = 1, .g = 1, .b = 1 } });
}

pub fn deinit_scene(alloc: std.mem.Allocator) void {
    scene.objects.deinit(alloc);
}

pub fn get_scene() *Scene {
    return &scene;
}

pub fn get_camera() *camera.Camera {
    return &cam;
}
