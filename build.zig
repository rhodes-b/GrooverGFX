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

    const scene3a = b.addExecutable(.{
        .name = "scene3a",
        .root_source_file = b.path("src/scene3a.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene3a.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene3a);

    const scene3b = b.addExecutable(.{
        .name = "scene3b",
        .root_source_file = b.path("src/scene3b.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene3b.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene3b);

    const scene3c = b.addExecutable(.{
        .name = "scene3c",
        .root_source_file = b.path("src/scene3c.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene3c.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene3c);

    const scene3d = b.addExecutable(.{
        .name = "scene3d",
        .root_source_file = b.path("src/scene3d.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene3d.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene3d);

    const scene3e = b.addExecutable(.{
        .name = "scene3e",
        .root_source_file = b.path("src/scene3e.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene3e.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene3e);

    const scene4 = b.addExecutable(.{
        .name = "scene4",
        .root_source_file = b.path("src/scene4.zig"),
        .target = target,
        .optimize = optimize,
    });

    scene4.linkLibrary(lib_groove_gfx);
    b.installArtifact(scene4);
}
