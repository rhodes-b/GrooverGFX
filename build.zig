const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const lib_groove_gfx = b.addStaticLibrary(.{
        .name = "groove-gfx",
        .root_source_file = b.path("src/root.zig"),
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(lib_groove_gfx);

    const scene0 = b.addExecutable(.{
        .name = "scene0",
        .root_source_file = b.path("src/scene0.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene0.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene0);
}
