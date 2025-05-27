const gfx_types = @import("gfx_types.zig");

pub const Material = struct { diffuse: gfx_types.Pixel, ambient: gfx_types.Pixel, specular: gfx_types.Pixel, shininess: f32, reflect: ?gfx_types.Pixel };

pub const BLACK_PLASTIC = Material{ .diffuse = .{ .r = 0.1, .g = 0.1, .b = 0.1 }, .ambient = .{ .r = 0.0, .g = 0.0, .b = 0.0 }, .specular = .{ .r = 0.5, .g = 0.5, .b = 0.5 }, .shininess = 32.0, .reflect = null };

pub const GREEN_PLASTIC = Material{ .diffuse = .{ .r = 0.0, .g = 0.6, .b = 0.0 }, .ambient = .{ .r = 0.0, .g = 0.3, .b = 0.0 }, .specular = .{ .r = 0.6, .g = 0.8, .b = 0.6 }, .shininess = 32.0, .reflect = null };

pub const RED_PLASTIC = Material{ .diffuse = .{ .r = 0.6, .g = 0.0, .b = 0.0 }, .ambient = .{ .r = 0.3, .g = 0.0, .b = 0.0 }, .specular = .{ .r = 0.8, .g = 0.6, .b = 0.6 }, .shininess = 32.0, .reflect = null };

pub const BRASS = Material{ .diffuse = .{ .r = 0.780392, .g = 0.568627, .b = 0.113725 }, .ambient = .{ .r = 0.329412, .g = 0.223529, .b = 0.027451 }, .specular = .{ .r = 0.992157, .g = 0.941176, .b = 0.807843 }, .shininess = 27.8974, .reflect = null };

pub const COPPER = Material{ .diffuse = .{ .r = 0.7038, .g = 0.27048, .b = 0.0828 }, .ambient = .{ .r = 0.19125, .g = 0.0735, .b = 0.0225 }, .specular = .{ .r = 0.256777, .g = 0.137622, .b = 0.086014 }, .shininess = 12.8, .reflect = null };

pub const GOLD = Material{ .diffuse = .{ .r = 0.75164, .g = 0.60648, .b = 0.22648 }, .ambient = .{ .r = 0.24725, .g = 0.1995, .b = 0.0745 }, .specular = .{ .r = 0.628281, .g = 0.555802, .b = 0.366065 }, .shininess = 51.2, .reflect = null };

pub const SILVER = Material{ .diffuse = .{ .r = 0.50754, .g = 0.50754, .b = 0.50754 }, .ambient = .{ .r = 0.19225, .g = 0.19225, .b = 0.19225 }, .specular = .{ .r = 0.508273, .g = 0.508273, .b = 0.508273 }, .shininess = 51.2, .reflect = null };

pub fn make_material(color: anytype) Material {
    if (@TypeOf(color) == Material) {
        return color;
    }
    return .{
        .diffuse = color,
        .ambient = color,
        .specular = .{ .r = 0.5, .g = 0.5, .b = 0.5 },
        .shininess = 30,
        .reflect = null,
    };
}
