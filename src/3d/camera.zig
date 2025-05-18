const std = @import("std");
const gfx_types = @import("gfx_types.zig");
const ray3d = @import("ray3d.zig");

pub const Camera = struct {
    const Self = @This();

    eye: gfx_types.Point(f32, 3) = .{ .vals = @splat(0) },
    window: [2]gfx_types.Point(f32, 2) = .{ .{ .vals = .{ -10, -10 } }, .{ .vals = .{ 10, 10 } } },
    distance: u16 = 10,
    dx: f32 = 0,
    dy: f32 = 0,

    pub fn set_perspective(self: *Self, hfov: f32, aspect: f32, distance: u16) void {
        self.distance = distance;
        const half_width = std.math.tan(std.math.degreesToRadians(hfov) / 2.0) * @as(f32, @floatFromInt(distance));
        self.window[0].vals[0] = -half_width;
        self.window[0].vals[1] = -half_width / aspect;
        self.window[1].vals[0] = half_width;
        self.window[1].vals[1] = half_width / aspect;
    }

    pub fn set_resolution(self: *Self, width: u16, height: u16) void {
        self.dx = (self.window[1].vals[0] - self.window[0].vals[0]) / @as(f32, @floatFromInt(width));
        self.dy = (self.window[1].vals[1] - self.window[0].vals[1]) / @as(f32, @floatFromInt(height));
    }

    pub fn ij_ray(self: *const Self, i: f32, j: f32) ray3d.Ray {
        const pt = gfx_types.Point(f32, 3){
            .vals = .{
                self.window[0].get_pos(0) + self.dx * (0.5 + i),
                self.window[0].get_pos(1) + self.dy * (0.5 + j),
                -@as(f32, @floatFromInt(self.distance)),
            },
        };

        const v = pt.sub(@as(*const gfx_types.Point(f32, 3), @ptrCast(&self.eye)));

        return .{ .start = self.eye, .dir = v };
    }
};

test "Horizonal View" {
    var cam = Camera{};
    cam.set_perspective(60, 1.333, 20);

    try std.testing.expectEqual(0, cam.eye.get_pos(0));
    try std.testing.expectEqual(0, cam.eye.get_pos(1));
    try std.testing.expectEqual(0, cam.eye.get_pos(2));

    try std.testing.expectEqual(-11.547005, cam.window[0].get_pos(0));
    try std.testing.expectEqual(-8.662419, cam.window[0].get_pos(1));
    try std.testing.expectEqual(11.547005, cam.window[1].get_pos(0));
    try std.testing.expectEqual(8.662419, cam.window[1].get_pos(1));
}

test "Resolution" {
    var cam = Camera{};
    cam.set_resolution(400, 300);

    const r1 = cam.ij_ray(-0.5, -0.5);
    try std.testing.expectEqual(0.00, r1.start.get_pos(0));
    try std.testing.expectEqual(0.00, r1.start.get_pos(1));
    try std.testing.expectEqual(0.00, r1.start.get_pos(2));

    try std.testing.expectEqual(-10.00, r1.dir.get_pos(0));
    try std.testing.expectEqual(-10.00, r1.dir.get_pos(1));
    try std.testing.expectEqual(-10.00, r1.dir.get_pos(2));

    const r2 = cam.ij_ray(399.5, 299.5);
    try std.testing.expectEqual(0.00, r2.start.get_pos(0));
    try std.testing.expectEqual(0.00, r2.start.get_pos(1));
    try std.testing.expectEqual(0.00, r2.start.get_pos(2));

    try std.testing.expectApproxEqAbs(10.00, r2.dir.get_pos(0), 0.00001);
    try std.testing.expectApproxEqAbs(10.00, r2.dir.get_pos(1), 0.00001);
    try std.testing.expectApproxEqAbs(-10.00, r2.dir.get_pos(2), 0.00001);

    const r3 = cam.ij_ray(0, 0);
    try std.testing.expectEqual(0.00, r3.start.get_pos(0));
    try std.testing.expectEqual(0.00, r3.start.get_pos(1));
    try std.testing.expectEqual(0.00, r3.start.get_pos(2));

    try std.testing.expectApproxEqAbs(-9.975, r3.dir.get_pos(0), 0.00001);
    try std.testing.expectApproxEqAbs(-9.96666666, r3.dir.get_pos(1), 0.00001);
    try std.testing.expectApproxEqAbs(-10.00, r3.dir.get_pos(2), 0.00001);

    const r4 = cam.ij_ray(399.0 / 2.0, 299.0 / 2.0);
    try std.testing.expectEqual(0.00, r4.start.get_pos(0));
    try std.testing.expectEqual(0.00, r4.start.get_pos(1));
    try std.testing.expectEqual(0.00, r4.start.get_pos(2));

    try std.testing.expectApproxEqAbs(0.00, r4.dir.get_pos(0), 0.00001);
    try std.testing.expectApproxEqAbs(0.00, r4.dir.get_pos(1), 0.00001);
    try std.testing.expectApproxEqAbs(-10.00, r4.dir.get_pos(2), 0.00001);
}
