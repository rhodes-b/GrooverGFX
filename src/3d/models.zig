const std = @import("std");
const gfx_types = @import("gfx_types.zig");
const math3d = @import("math3d.zig");
const ray3d = @import("ray3d.zig");

// void free_record(struct Record* r);
pub const Record = struct {
    points: []gfx_types.Point(f32, 3),
    color: gfx_types.Pixel,
    normal: math3d.Vec(3),
    time: f32,
    // uvn ?
    // texture ?
};

fn circ_points(center: *const gfx_types.Point(f32, 2), radius: f64, pts: []gfx_types.Point(f32, 2)) void {
    const dtheta: f64 = std.math.tau / @as(f64, @floatFromInt(pts.len - 1));

    const x = center.get_pos(0);
    const y = center.get_pos(1);

    for (0..pts.len - 1) |i| {
        pts[i] = .{ .vals = .{
            @floatCast(radius * @cos(@as(f64, @floatFromInt(i)) * dtheta) + x),
            @floatCast(radius * @sin(@as(f64, @floatFromInt(i)) * dtheta) + y),
        } };
    }
    pts[pts.len - 1] = pts[0];
}

pub const Sphere = struct {
    const Self = @This();

    pos: gfx_types.Point(f32, 3),
    radius: f32,
    color: gfx_types.Pixel,
    nlat: u8,
    nlong: u8,
    bands: []gfx_types.Point(f32, 3),
    axis: math3d.Vec(3),
    northpole: gfx_types.Point(f32, 3),
    southpole: gfx_types.Point(f32, 3),

    pub fn init(alloc: std.mem.Allocator, pos: gfx_types.Point(f32, 3), radius: f32, color: gfx_types.Pixel, nlat: u8, nlong: u8) !Self {
        var s: Self = undefined;
        s.pos = pos;
        s.radius = radius;
        s.color = color;
        s.nlat = nlat;
        s.nlong = nlong;
        try s.make_bands(alloc);
        s.axis = math3d.Vec(3){ .vals = .{ 0, radius, 0 } };
        s.northpole = s.axis.add_pt(s.pos);
        s.southpole = s.axis.sub_pt(s.pos);

        return s;
    }

    pub fn deinit(self: *const Self, alloc: std.mem.Allocator) void {
        alloc.free(self.bands);
    }

    pub fn iter_polygons(self: *const Self, alloc: std.mem.Allocator) std.ArrayList(Record) {
        _ = self;
        _ = alloc;
        @compileError("TODO");
    }

    pub fn intersect(self: *const Self, alloc: std.mem.Allocator, r: *ray3d.Ray, i: *ray3d.Interval, info: *Record) !bool {
        const a = r.dir.mag_sq();
        if (a == 0) {
            return false;
        }

        const rp_vec = r.start.sub(&self.pos);
        const b = 2 * r.dir.dot(&rp_vec);
        const c = rp_vec.mag_sq() - self.radius * self.radius;
        const discrim = b * b - 4.0 * a * c;
        if (discrim < 0) {
            return false;
        }

        const t1 = (-b - @sqrt(discrim)) / (2.0 * a);
        if (i.contains(t1)) {
            try self.set_info(alloc, r, t1, info);
            return true;
        }
        const t2 = (-b + @sqrt(discrim)) / (2.0 * a);
        if (i.contains(t2)) {
            try self.set_info(alloc, r, t2, info);
            return true;
        }
        return false;
    }

    fn set_info(self: *const Self, alloc: std.mem.Allocator, r: *ray3d.Ray, t: f32, info: *Record) !void {
        info.time = t;
        // TODO: do I need this? (Can i just always free it?)
        // if(info->pts != NULL) {
        //     free(info->pts);
        // }
        info.points = try alloc.alloc(gfx_types.Point(f32, 3), 1);
        info.points[0] = r.point_at(t);

        info.color = self.color;
        const tmp = info.points[0].sub(&self.pos);
        info.normal = tmp.normalized();
    }

    fn get_bands(self: *const Self, index: u16) u16 {
        return index * (self.nlong + 1);
    }

    fn make_bands(self: *Self, alloc: std.mem.Allocator) !void {
        self.bands = try alloc.alloc(gfx_types.Point(f32, 3), @as(usize, @intCast(self.nlat)) * @as(usize, @intCast(self.nlong + 1)));

        const cx = self.pos.get_pos(0);
        const cy = self.pos.get_pos(1);
        const cz = self.pos.get_pos(2);

        var theta: f64 = std.math.pi / 2.0;
        const dtheta: f64 = std.math.pi / @as(f64, @floatFromInt(self.nlat + 1));

        for (0..self.nlat) |i| {
            const i_u16: u16 = @intCast(i);
            theta += dtheta;
            const r = self.radius * @cos(theta);
            const y = self.radius * @sin(theta) + cy;
            var pts: []gfx_types.Point(f32, 2) = try alloc.alloc(gfx_types.Point(f32, 2), self.nlong + 1);
            defer alloc.free(pts);
            const center = gfx_types.Point(f32, 2){ .vals = .{ cx, cz } };
            circ_points(&center, r, pts);
            for (0..self.nlong + 1) |j| {
                self.bands[self.get_bands(i_u16) + j] = .{ .vals = .{ pts[j].get_pos(0), @floatCast(y), pts[j].get_pos(1) } };
            }
        }
    }
};

pub const Box = struct {
    const Self = @This();

    pos: gfx_types.Point(f32, 3),
    size: math3d.Vec(3),
    color: gfx_types.Pixel,
    planes: [3]gfx_types.Point(f32, 2),

    pub fn init(pos: gfx_types.Point(f32, 3), size: math3d.Vec(3), color: gfx_types.Pixel) Self {
        var b: Self = undefined;
        b.pos = pos;
        b.size = size;
        b.color = color;
        b.make_planes();
        return b;
    }

    pub fn iter_polygons(self: *const Self, alloc: std.mem.Allocator) std.ArrayList(Record) {
        _ = self;
        _ = alloc;
        @compileError("TODO");
    }

    // TODO: should we use alloc for the record?
    pub fn intersect(self: *const Self, alloc: std.mem.Allocator, r: *ray3d.Ray, i: *ray3d.Interval, info: *Record) !bool {
        var hit = false;
        for (0..3) |a| {
            if (r.dir.get_pos(@intCast(a)) == 0.00) {
                continue;
            }
            // low and high
            for (0..2) |lh| {
                const plane_xy = if (lh == 0) self.planes[a].get_pos(0) else self.planes[a].get_pos(1);
                const ray_start_xyz = if (a == 0) r.start.get_pos(0) else if (a == 1) r.start.get_pos(1) else r.start.get_pos(2);
                const t = (plane_xy - ray_start_xyz) / r.dir.get_pos(@intCast(a));
                if (!i.contains(t)) {
                    continue;
                }

                const tmp = r.point_at(t);
                if (self.in_rect(&tmp, @intCast(a))) {
                    hit = true;
                    i.high = t;
                    info.time = t;
                    info.points = try alloc.alloc(gfx_types.Point(f32, 3), 1);
                    info.points[0] = tmp;
                    info.color = self.color;
                    info.normal = math3d.Vec(3){ .vals = .{ 0, 0, 0 } };
                    info.normal.set_pos(@intCast(a), if (lh == 0) -1 else 1);
                }
            }
        }
        return hit;
    }

    fn in_rect(self: *const Self, p: *const gfx_types.Point(f32, 3), axis: u8) bool {
        // loop over all planes
        for (0..3) |a| {
            if (a == axis) {
                continue;
            }
            const low = self.planes[a].get_pos(0);
            const high = self.planes[a].get_pos(1);
            const xyz = if (a == 0) p.get_pos(0) else if (a == 1) p.get_pos(1) else p.get_pos(2);
            if (!((low <= xyz) and (xyz <= high))) {
                return false;
            }
        }
        return true;
    }

    fn make_planes(self: *Self) void {
        const sx = self.size.get_pos(0) * 0.5;
        const sy = self.size.get_pos(1) * 0.5;
        const sz = self.size.get_pos(2) * 0.5;

        const cx = self.pos.get_pos(0);
        const cy = self.pos.get_pos(1);
        const cz = self.pos.get_pos(2);

        self.planes[0] = .{ .vals = .{ cx - sx, cx + sx } };
        self.planes[1] = .{ .vals = .{ cy - sy, cy + sy } };
        self.planes[2] = .{ .vals = .{ cz - sz, cz + sz } };
    }
};

pub const Shape = union(enum) {
    box: Box,
    sphere: Sphere,
};

pub const Group = struct {
    const Self = @This();
    objects: std.ArrayList(Shape),

    pub fn init(alloc: std.mem.Allocator) !Self {
        const arr_list = std.ArrayList(Shape).init(alloc);
        return .{
            .objects = arr_list,
        };
    }

    pub fn deinit(self: *const Self, alloc: std.mem.Allocator) void {
        for (self.objects.items) |obj| {
            switch (obj) {
                .box => continue,
                .sphere => |s| s.deinit(alloc),
            }
        }
        self.objects.deinit();
    }

    pub fn add(self: *Self, model: Shape) !void {
        try self.objects.append(model);
    }

    pub fn iter_polygons(self: *const Self, alloc: std.mem.Allocator) std.ArrayList(Record) {
        _ = self;
        _ = alloc;
    }

    pub fn intersect(self: *const Self, alloc: std.mem.Allocator, r: *ray3d.Ray, i: *ray3d.Interval, info: *Record) !bool {
        var hit = false;
        for (self.objects.items) |obj| {
            switch (obj) {
                .box => |b| if (try b.intersect(alloc, r, i, info)) {
                    hit = true;
                    i.high = info.time;
                },
                .sphere => |s| if (try s.intersect(alloc, r, i, info)) {
                    hit = true;
                    i.high = info.time;
                },
            }
        }
        return hit;
    }
};

test "Create Sphere Basic" {
    const alloc = std.testing.allocator;

    var s = try Sphere.init(alloc, .{ .vals = .{ 0, 0, 0 } }, 1, .{ .r = 0, .g = 1, .b = 0 }, 7, 15);
    defer s.deinit(alloc);

    try std.testing.expectEqual(gfx_types.Point(f32, 3){ .vals = .{ -0.3826834323650897, 0.9238795325112867, 0 } }, s.bands[0]);
    try std.testing.expectEqual(gfx_types.Point(f32, 3){ .vals = .{ -0.3495987113522076, -0.9238795325112865, 0.15565137464090215 } }, s.bands[s.bands.len - 2]);
}

test "Create Box Basic" {
    const b = Box.init(.{ .vals = .{ 0, 0, 0 } }, .{ .vals = .{ 1, 1, 1 } }, .{ .r = 1, .g = 0, .b = 0 });

    try std.testing.expectEqual(gfx_types.Point(f32, 2){ .vals = .{ -0.5, 0.5 } }, b.planes[0]);
    try std.testing.expectEqual(gfx_types.Point(f32, 2){ .vals = .{ -0.5, 0.5 } }, b.planes[1]);
    try std.testing.expectEqual(gfx_types.Point(f32, 2){ .vals = .{ -0.5, 0.5 } }, b.planes[2]);
}
