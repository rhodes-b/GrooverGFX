const gfx_types = @import("gfx_types.zig");

pub fn Vec(comptime n: u8) type {
    return struct {
        const Self = @This();

        vals: [n]f32,

        pub fn get_pos(self: *const Self, pos: u8) f32 {
            if (pos > n) {
                unreachable;
            }
            return self.vals[pos];
        }

        pub fn set_pos(self: *Self, pos: u8, val: f32) void {
            if (pos > n) {
                unreachable;
            }
            self.vals[pos] = val;
        }

        pub fn mul(self: *const Self, s: f32) Self {
            var vals: [n]f32 = undefined;
            inline for (0..n) |i| {
                vals[i] = self.vals[i] * s;
            }

            return Self{
                .vals = vals,
            };
        }

        pub fn add(self: *const Self, other: *const Self) Self {
            var vals: [n]f32 = undefined;
            inline for (0..n) |i| {
                vals[i] = self.vals[i] + other.vals[i];
            }

            return Self{
                .vals = vals,
            };
        }

        pub fn sub(self: *const Self, other: *const Self) Self {
            var vals: [n]f32 = undefined;
            inline for (0..n) |i| {
                vals[i] = self.vals[i] - other.vals[i];
            }

            return Self{
                .vals = vals,
            };
        }

        pub fn neg(self: *const Self) Self {
            var vals: [n]f32 = undefined;
            inline for (0..n) |i| {
                vals[i] = -self.vals[i];
            }

            return Self{
                .vals = vals,
            };
        }

        pub fn dot(self: *const Self, other: *const Self) f32 {
            var result: f32 = 0;
            inline for (0..n) |i| {
                result += self.vals[i] * other.vals[i];
            }

            return result;
        }

        pub fn cross(self: *const Self, other: *const Self) Self {
            if (n != 3) {
                unreachable;
            }

            return Self{ .vals = .{
                self.vals[1] * other.vals[2] - other.vals[1] * self.vals[2],
                self.vals[2] * other.vals[0] - other.vals[2] * self.vals[0],
                self.vals[0] * other.vals[1] - other.vals[0] * self.vals[1],
            } };
        }

        pub fn reflect(self: *const Self, norm: *const Self) Self {
            const tmp = norm.mul(2 * self.dot(norm));
            return self.sub(&tmp);
        }

        pub fn mag(self: *const Self) f32 {
            var sum: f32 = 0;
            inline for (0..n) |i| {
                sum += self.vals[i] * self.vals[i];
            }

            return @sqrt(sum);
        }

        pub fn mag_sq(self: *const Self) f32 {
            var sum: f32 = 0;
            inline for (0..n) |i| {
                sum += self.vals[i] * self.vals[i];
            }

            return sum;
        }

        pub fn normalized(self: *const Self) Self {
            const vec_mag = self.mag();
            var vals: [n]f32 = undefined;
            inline for (0..n) |i| {
                vals[i] = self.vals[i] / vec_mag;
            }

            return Self{
                .vals = vals,
            };
        }

        pub fn normalize(self: *Self) void {
            const vec_mag = self.mag(self);
            inline for (0..n) |i| {
                self.vals[i] /= vec_mag;
            }
        }

        pub fn add_pt(self: *const Self, p: gfx_types.Point(f32, n)) gfx_types.Point(f32, n) {
            var vals: [n]f32 = undefined;
            inline for (0..n) |i| {
                vals[i] = self.vals[i] + p.vals[i];
            }

            return gfx_types.Point(f32, n){ .vals = vals };
        }

        pub fn sub_pt(self: *const Self, p: gfx_types.Point(f32, n)) gfx_types.Point(f32, n) {
            var vals: [n]f32 = undefined;
            inline for (0..n) |i| {
                vals[i] = self.vals[i] - p.vals[i];
            }

            return gfx_types.Point(f32, n){ .vals = vals };
        }
    };
}
