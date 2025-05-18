const std = @import("std");
const gfx_types = @import("gfx_types.zig");

const Matrix = struct {
    const Self = @This();
    rows: u8,
    cols: u8,
    vals: []f32,

    pub fn init(alloc: std.mem.Allocator, rows: u8, cols: u8) !Self {
        const buff = try alloc.alloc(f32, rows * cols);
        return .{
            .rows = rows,
            .cols = cols,
            .vals = buff,
        };
    }

    pub fn deinit(self: *const Self, alloc: std.mem.Allocator) void {
        alloc.free(self.vals);
    }

    pub fn print(self: *const Self) void {
        std.debug.print("rows: {}, cols: {}\n", .{ self.rows, self.cols });
        std.debug.print("[", .{});
        for (0..self.rows) |r| {
            const i: u8 = @intCast(r);
            std.debug.print("\n", .{});
            for (0..self.cols) |c| {
                const j: u8 = @intCast(c);
                const val = self._get(i, j) catch 0.0;
                std.debug.print("{} ", .{val});
            }
        }
        std.debug.print("\n", .{});
        std.debug.print("]\n", .{});
    }

    inline fn check_bounds(self: *const Self, r: u8, c: u8) bool {
        return (r < self.rows) and (c < self.cols);
    }

    inline fn _get(self: *const Self, r: u8, c: u8) !f32 {
        if (!self.check_bounds(r, c)) {
            std.log.err("r:{d}, c:{}", .{ r, c });
            return error.INVALID_BOUNDS;
        }
        const loc = (r * self.cols) + c;
        return self.vals[loc];
    }

    inline fn _set(self: *Self, r: u8, c: u8, val: f32) !void {
        if (!self.check_bounds(r, c)) {
            std.log.err("r:{d}, c:{}, rows:{}, cols:{}", .{ r, c, self.rows, self.cols });
            return error.INVALID_BOUNDS;
        }
        const loc = (r * self.cols) + c;
        self.vals[loc] = val;
    }

    // m[r][c]
    pub fn get(self: *const Self, r: u8, c: u8) !f32 {
        return try self._get(r, c);
    }

    // m[r][c] = val
    pub fn set(self: *Self, r: u8, c: u8, val: f32) !void {
        return try self._set(r, c, val);
    }

    // Perform matrix dot product
    // M1:NxM, M2:AxN = M3:AxM
    // [[1, 2], [3, 4], [5, 6]] * [[5, 6, 7], [8, 9, 10]] = 170
    pub fn dot(self: *const Self, other: *const Self) f32 {
        var dot_prod: f32 = 0;
        for (0..self.rows) |r| {
            const i: u8 = @intCast(r);
            for (0..self.cols) |c| {
                const j: u8 = @intCast(c);
                dot_prod += try self._get(i, j) * other._get(i, j);
            }
        }

        return dot_prod;
    }

    // Perform matrix multiplication
    // M1:NxM, M2:AxN = M3:MxN
    // [[1, 2], [3, 4], [5, 6]] * [[5, 6, 7], [8, 9, 10]] = [[21, 24, 27], [47, 54, 61], [73, 84, 95]]
    pub fn mul(self: *const Self, alloc: std.mem.Allocator, other: *const Self) !Self {
        var m3 = try Self.init(alloc, self.rows, other.cols);
        for (0..self.rows) |r| {
            const i: u8 = @intCast(r);
            for (0..other.cols) |c| {
                const j: u8 = @intCast(c);
                try m3._set(i, j, 0);
                for (0..self.cols) |k| {
                    const w: u8 = @intCast(k);
                    const val = try m3.get(i, j) + try self.get(i, w) * try other.get(w, j);
                    try m3._set(i, j, val);
                }
            }
        }

        return m3;
    }

    // Perform Matrix transpose
    // M:MxN = T:NxM
    // [[1, 2], [3, 4], [5, 6]] = [[1, 3, 5], [2, 4, 6]]
    pub fn transpose(self: *const Self, alloc: std.mem.Allocator) !Self {
        var t = try Self.init(alloc, self.cols, self.rows);
        for (0..self.rows) |r| {
            const i: u8 = @intCast(r);
            for (0..self.cols) |c| {
                const j: u8 = @intCast(c);
                const val = try self._get(i, j);
                try t._set(j, i, val);
            }
        }

        return t;
    }

    // Apply M1 to sequence M2
    // M1:MxN, sequence is a vector of length M
    pub fn apply(self: *Self, alloc: std.mem.Allocator, seq: []const f32, res: []f32) !void {
        if (self.rows != seq.len) {
            unreachable;
        }
        var tmp1 = try Self.init(alloc, 1, self.rows);
        var tmp2 = try tmp1.transpose(alloc);
        for (0..seq.len) |r| {
            const i: u8 = @intCast(r);
            try tmp2._set(i, 0, seq[i]);
        }

        var tmp3 = try self.mul(alloc, @as(*const Self, @ptrCast(&tmp2)));
        for (0..tmp3.rows) |r| {
            const i: u8 = @intCast(r);
            res[i] = try tmp3._get(i, 0);
        }

        tmp1.deinit(alloc);
        tmp2.deinit(alloc);
        tmp3.deinit(alloc);
    }

    // [[1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]]
    pub fn make_identity(alloc: std.mem.Allocator, size: u8) !Self {
        var m = try Self.init(alloc, size, size);
        for (0..size) |i| {
            for (0..size) |j| {
                if (i != j) continue;
                try m.set(i, j, 1);
            }
        }
        return m;
    }

    // [[sx, 0.0, 0.0], [0.0, sy, 0.0], [0.0, 0.0, 1.0]]
    pub fn scale_xy(alloc: std.mem.Allocator, sx: f32, sy: f32) !Self {
        var m = try Self.init(alloc, 3, 3);
        try m.set(0, 0, sx);
        try m.set(1, 1, sy);
        try m.set(2, 2, 1.0);
        return m;
    }

    pub fn translate_xy(alloc: std.mem.Allocator, dx: f32, dy: f32) !Self {
        var m = try Self.make_identity(alloc, 3);
        try m.set(0, 2, dx);
        try m.set(1, 2, dy);
        return m;
    }

    // [[cos(radian(angle)), -sin(radian(angle)), 0.0], [sin(radian(angle)), cos(radian(angle)), 0.0], [0.0, 0.0, 1.0]]
    pub fn rotate_degrees_xy(alloc: std.mem.Allocator, angle: f32) !Self {
        var m = try Self.init(alloc, 3, 3);
        try m.set(0, 0, @cos(std.math.degreesToRadians(angle)));
        try m.set(0, 1, -@sin(std.math.degreesToRadians(angle)));
        try m.set(1, 0, @sin(std.math.degreesToRadians(angle)));
        try m.set(1, 1, @cos(std.math.degreesToRadians(angle)));
        try m.set(2, 2, 1);
        return m;
    }

    // // [[1.0, sx, 0.0], [sy, 1.0, 0.0], [0.0, 0.0, 1.0]]
    pub fn shear_xy(alloc: std.mem.Allocator, sx: f32, sy: f32) !Self {
        var m = try Self.make_identity(alloc, 3);
        try m.set(0, 1, sx);
        try m.set(1, 0, sy);
        return m;
    }

    // [[-1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]]
    pub fn reflect_x(alloc: std.mem.Allocator) !Self {
        var m = try Self.make_identity(alloc, 3);
        try m.set(0, 0, -1);
        return m;
    }

    // [[1.0, 0.0, 0.0], [0.0, -1.0, 0.0], [0.0, 0.0, 1.0]]
    pub fn reflect_y(alloc: std.mem.Allocator) !Self {
        var m = try Self.make_identity(alloc, 3);
        try m.set(1, 1, -1);
        return m;
    }

    // b1 and b2 are boxes {left, bottom, right, top} which are point pairs (left, bottom) (right, top)
    // gives the transformation from box1 to box2
    pub fn make_window(alloc: std.mem.Allocator, b1: [2]gfx_types.Point(f32, 2), b2: [2]gfx_types.Point(f32, 2)) !Self {
        const scale_x = @abs(b2[1].get_pos(0) - b2[0].get_pos(0)) / @abs(b1[1].get_pos(0) - b1[0].get_pos(0));
        const scale_y = @abs(b2[1].get_pos(1) - b2[0].get_pos(1)) / @abs(b1[1].get_pos(1) - b1[0].get_pos(1));

        const tmp0 = try Self.translate_xy(alloc, b2[0].get_pos(0), b2[0].get_pos(1));
        const tmp1 = try Self.scale_xy(alloc, scale_x, scale_y);

        const tmp2 = try tmp0.mul(alloc, tmp1);

        const tmp3 = try Self.translate_xy(alloc, -b1[0].get_pos(0), -b1[0].get_pos(1));

        const res = tmp2.mul(alloc, tmp3);

        tmp0.deinit(alloc);
        tmp1.deinit(alloc);
        tmp2.deinit(alloc);
        tmp3.deinit(alloc);

        return res;
    }
};

test "Apply 1" {
    var debug_alloc = std.heap.DebugAllocator(.{}).init;
    const alloc = debug_alloc.allocator();

    var m = try Matrix.init(alloc, 2, 2);
    try m.set(0, 0, 1);
    try m.set(0, 1, 2);
    try m.set(1, 0, 3);
    try m.set(1, 1, 4);

    const seq = [_]f32{ -1, 1 };
    var res: [2]f32 = undefined;
    try m.apply(alloc, &seq, &res);

    try std.testing.expectEqual(1, res[0]);
    try std.testing.expectEqual(1, res[1]);
}

test "Apply 2" {
    var debug_alloc = std.heap.DebugAllocator(.{}).init;
    const alloc = debug_alloc.allocator();

    var m = try Matrix.init(alloc, 2, 2);
    try m.set(0, 0, -2);
    try m.set(0, 1, 3);
    try m.set(1, 0, 1);
    try m.set(1, 1, -1);

    const seq = [_]f32{ -1, 1 };
    var res: [2]f32 = undefined;
    try m.apply(alloc, &seq, &res);

    try std.testing.expectEqual(5, res[0]);
    try std.testing.expectEqual(-2, res[1]);
}
