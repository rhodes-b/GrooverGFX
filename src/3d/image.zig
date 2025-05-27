const std = @import("std");
const gfx_types = @import("gfx_types.zig");

const PPM_HEADER = "P6\n";

pub const Image = struct {
    const Self = @This();

    pixels: []u8,
    width: u16,
    height: u16,

    pub fn init(alloc: std.mem.Allocator, width: u16, height: u16) !Self {
        const pix_bytes = try alloc.alloc(u8, (@as(usize, width) * @as(usize, height)) * 3);
        @memset(pix_bytes, 255);
        return Self{ .pixels = pix_bytes, .width = width, .height = height };
    }

    pub fn deinit(self: *const Self, alloc: std.mem.Allocator) void {
        alloc.free(self.pixels);
    }

    pub fn get_pixel(self: *const Self, pt: gfx_types.Point(i16, 2)) gfx_types.Pixel {
        const arr_pos = self.locate_pixel_index(pt);
        return .{
            .r = @floatFromInt(self.pixels[arr_pos + 0]),
            .g = @floatFromInt(self.pixels[arr_pos + 1]),
            .b = @floatFromInt(self.pixels[arr_pos + 2]),
        };
    }

    pub fn set_pixel(self: *Self, pt: gfx_types.Point(i16, 2), pix: gfx_types.Pixel) void {
        if ((pt.get_pos(0) >= 0) and (pt.get_pos(0) <= self.width) and (pt.get_pos(1) >= 0) and (pt.get_pos(1) < self.height)) {
            const arr_pos = self.locate_pixel_index(pt);
            self.pixels[arr_pos + 0] = @intFromFloat(pix.r);
            self.pixels[arr_pos + 1] = @intFromFloat(pix.g);
            self.pixels[arr_pos + 2] = @intFromFloat(pix.b);
        }
    }

    pub fn load(alloc: std.mem.Allocator, fname: []const u8) !Self {
        var f = try std.fs.openFileAbsolute(fname, .{});
        defer f.close();

        var header: [PPM_HEADER.len]u8 = @splat(0);
        _ = try f.read(&header);
        if (!std.mem.eql(u8, PPM_HEADER, &header)) {
            return error.WRONG_HEADER;
        }

        var width_buf: [32]u8 = @splat(0);
        var width_stream = std.io.FixedBufferStream([]u8){ .buffer = &width_buf, .pos = 0 };
        const width_writer = width_stream.writer();
        const file_reader = f.reader();
        try file_reader.streamUntilDelimiter(width_writer, ' ', null);
        var len: u16 = 0;
        for (0..width_buf.len) |i| {
            len += if (width_buf[i] != 0) 1 else break;
        }
        const width = try std.fmt.parseInt(u16, width_buf[0..len], 10);

        var height_buf: [32]u8 = @splat(0);
        var height_stream = std.io.FixedBufferStream([]u8){ .buffer = &height_buf, .pos = 0 };
        const height_writer = height_stream.writer();
        try f.reader().streamUntilDelimiter(height_writer, '\n', null);
        len = 0;
        for (0..height_buf.len) |i| {
            len += if (height_buf[i] != 0) 1 else break;
        }
        const height = try std.fmt.parseInt(u16, height_buf[0..len], 10);

        var max_pixel_val: [4]u8 = @splat(0);
        _ = try f.read(&max_pixel_val);
        if (!std.mem.eql(u8, &max_pixel_val, "255\n")) {
            return error.INVALID_MAX_INTENSITY;
        }

        const pix_buf = try alloc.alloc(u8, @as(usize, width) * @as(usize, height) * 3);
        // TODO: check correct number of bytes was read
        _ = try f.readAll(pix_buf);

        return Image{
            .pixels = pix_buf,
            .width = width,
            .height = height,
        };
    }

    pub fn save(self: *const Self, fname: []const u8) !void {
        var f = try std.fs.createFileAbsolute(fname, .{});
        defer f.close();
        const f_write = f.writer();
        try f_write.print(PPM_HEADER, .{});
        try f_write.print("{d} {d}\n", .{ self.width, self.height });
        try f_write.print("255\n", .{});
        try f.writeAll(self.pixels);
    }

    fn locate_pixel_index(self: *const Self, pt: gfx_types.Point(i16, 2)) usize {
        // calle checks that pt.x/y are >= 0 so abs is safe to convert the type
        return (@as(usize, self.height - 1 - @abs(pt.get_pos(1))) * 3 * self.width) + (@abs(pt.get_pos(0)) * 3);
    }
};

test "Image Dimensions" {
    const alloc = std.testing.allocator;

    const width = 640;
    const height = 480;

    const img = try Image.init(alloc, width, height);
    defer img.deinit(alloc);
    try std.testing.expectEqual(img.width, width);
    try std.testing.expectEqual(img.height, height);
}

test "Image get pixel" {
    const alloc = std.testing.allocator;

    const img = try Image.init(alloc, 320, 240);
    defer img.deinit(alloc);
    const val = img.get_pixel(gfx_types.Point(i16, 2){ .vals = .{ 200, 200 } });
    try std.testing.expectEqual(255, val.r);
    try std.testing.expectEqual(255, val.g);
    try std.testing.expectEqual(255, val.b);
}

test "Image set pixel" {
    const alloc = std.testing.allocator;

    var img = try Image.init(alloc, 320, 240);
    defer img.deinit(alloc);
    img.set_pixel(.{ .vals = .{ 200, 200 } }, .{ .r = 255, .g = 0, .b = 0 });
    const val = img.get_pixel(gfx_types.Point(i16, 2){ .vals = .{ 200, 200 } });
    try std.testing.expectEqual(255, val.r);
    try std.testing.expectEqual(0, val.g);
    try std.testing.expectEqual(0, val.b);
}

test "Image load" {
    const alloc = std.testing.allocator;

    const test_img_path = try std.fs.cwd().realpathAlloc(alloc, "src/assets/wartburg.ppm");
    defer alloc.free(test_img_path);
    const img = try Image.load(alloc, test_img_path);
    defer img.deinit(alloc);
    try std.testing.expectEqual(640, img.width);
    try std.testing.expectEqual(470, img.height);
    const val = img.get_pixel(.{ .vals = .{ 350, 220 } });
    try std.testing.expectEqual(148, val.r);
    try std.testing.expectEqual(103, val.g);
    try std.testing.expectEqual(82, val.b);
}
