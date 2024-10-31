
const std = @import("std");

pub const Image = struct {
    pixels: []u8,
    width: u16,
    height: u16,

    pub fn init(width: u16, height: u16, allocator: *std.mem.Allocator) Image {
        return Image {
            .width = width,
            .height = height,
            .pixels = try allocator.alloc(u8, width*height*3)
        };
    }

    pub fn deinit(self: *Image, allocator: *std.mem.Allocator) void {
          allocator.free(self.pixels);
    }

    pub fn get(self: *Image) void {
        _ = self;
    }

    pub fn set(self: *Image) void {
        _ = self;
    }

    pub fn load(self: *Image, fname: []const u8) void {
        _ = self;
        _ = fname;
    }

    pub fn save(self: *Image, fname : []const u8) void {
        _ = self;
        _ = fname;
    }

};

