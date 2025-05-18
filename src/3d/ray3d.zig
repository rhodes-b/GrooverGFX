const std = @import("std");
const gfx_types = @import("gfx_types.zig");
const math3d = @import("math3d.zig");

pub const Ray = struct {
    const Self = @This();

    start: gfx_types.Point(f32, 3),
    dir: math3d.Vec(3),

    pub fn point_at(self: *const Self, t: f32) gfx_types.Point(f32, 3) {
        const v = self.dir.mul(t);
        return v.add_pt(self.start);
    }
};

pub const Interval = struct {
    const Self = @This();

    low: f32,
    high: f32,

    pub fn contains(self: *const Self, t: f32) bool {
        return (self.low < t) and (t < self.high);
    }

    pub fn set(self: *Self, low: f32, high: f32) void {
        self.low = low;
        self.high = high;
    }
};

test "Ray at" {
    const r = Ray{
        .start = gfx_types.Point(f32, 3){ .vals = .{ 0, 1, 2 } },
        .dir = math3d.Vec(3){ .vals = .{ 1, 2, 3 } },
    };

    const p1 = r.point_at(0);
    try std.testing.expectEqual(0.00, p1.get_pos(0));
    try std.testing.expectEqual(1.00, p1.get_pos(1));
    try std.testing.expectEqual(2.00, p1.get_pos(2));

    const p2 = r.point_at(1);
    try std.testing.expectEqual(1.00, p2.get_pos(0));
    try std.testing.expectEqual(3.00, p2.get_pos(1));
    try std.testing.expectEqual(5.00, p2.get_pos(2));

    const p3 = r.point_at(3.75);
    try std.testing.expectEqual(3.75, p3.get_pos(0));
    try std.testing.expectEqual(8.50, p3.get_pos(1));
    try std.testing.expectEqual(13.25, p3.get_pos(2));
}
