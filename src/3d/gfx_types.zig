const std = @import("std");
const math3d = @import("math3d.zig");

pub fn Point(comptime T: type, comptime n: u8) type {
    return struct {
        const Self = @This();

        vals: [n]T,

        pub inline fn get_pos(self: *const Self, pos: u8) T {
            if (pos > n) {
                unreachable;
            }
            return self.vals[pos];
        }

        pub inline fn set_pos(self: *Self, pos: u8, val: T) void {
            if (pos > n) {
                unreachable;
            }
            self.vals[pos] = val;
        }

        // TODO: have inline funcs for x,y,z?

        pub fn sub(self: *const Self, other: *const Self) math3d.Vec(n) {
            var vals: [n]f32 = undefined;
            inline for (0..n) |i| {
                if (@typeInfo(T) == .int) {
                    vals[i] = @floatFromInt(self.vals[i] - other.vals[i]);
                } else {
                    vals[i] = self.vals[i] - other.vals[i];
                }
            }

            return math3d.Vec(n){ .vals = vals };
        }

        pub fn sub_vec(self: *const Self, v: *const math3d.Vec(n)) Self {
            if (@typeInfo(T) != .float) {
                @compileLog("Invalid Point for sub_vec");
            }
            var vals: [n]T = undefined;
            inline for (0..n) |i| {
                vals[i] = self.vals[i] - v.vals[i];
            }

            return Self{
                .vals = vals,
            };
        }
    };
}

pub const Pixel = struct {
    const Self = @This();

    r: f32,
    g: f32,
    b: f32,

    pub fn quantize(self: *const Self, val: u8) Self {
        return .{
            .r = @min(self.r * @as(f32, @floatFromInt(val)), @as(f32, @floatFromInt(val))),
            .g = @min(self.g * @as(f32, @floatFromInt(val)), @as(f32, @floatFromInt(val))),
            .b = @min(self.b * @as(f32, @floatFromInt(val)), @as(f32, @floatFromInt(val))),
        };
    }

    pub fn mul(self: *const Self, val: anytype) Self {
        if (@TypeOf(val) == Self) {
            return .{
                .r = self.r * val.r,
                .g = self.g * val.g,
                .b = self.b * val.b,
            };
        } else {
            return .{
                .r = self.r * val,
                .g = self.g * val,
                .b = self.b * val,
            };
        }
    }

    pub fn add(self: *const Self, other: *const Self) Self {
        return .{
            .r = self.r + other.r,
            .g = self.g + other.g,
            .b = self.b + other.b,
        };
    }
};
