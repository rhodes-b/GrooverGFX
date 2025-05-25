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

    const scene1 = b.addExecutable(.{
        .name = "scene1",
        .root_source_file = b.path("src/scene1.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene1.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene1);

    const scene2 = b.addExecutable(.{
        .name = "scene2",
        .root_source_file = b.path("src/scene2.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene2.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene2);

    const scene3 = b.addExecutable(.{
        .name = "scene3",
        .root_source_file = b.path("src/scene3.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene3.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene3);

    const scene4 = b.addExecutable(.{
        .name = "scene4",
        .root_source_file = b.path("src/scene4.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene4.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene4);
}
