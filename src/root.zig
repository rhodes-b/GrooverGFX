pub const camera = @import("3d/camera.zig");
pub const gfx_types = @import("3d/gfx_types.zig");
pub const image = @import("3d/image.zig");
pub const materials = @import("3d/materials.zig");
pub const math3d = @import("3d/math3d.zig");
pub const matrix = @import("3d/matrix.zig");
pub const models = @import("3d/models.zig");
pub const ray3d = @import("3d/ray3d.zig");
pub const render_ray = @import("3d/render_ray.zig");
pub const scenedef = @import("3d/scenedef.zig");

test {
    _ = camera;
    _ = gfx_types;
    _ = image;
    _ = math3d;
    _ = matrix;
    _ = models;
    _ = ray3d;
    _ = render_ray;
    _ = scenedef;
}
