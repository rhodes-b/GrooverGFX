
const std = @import("std");

pub fn build(b: *std.Build) void {

    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const lib_groove_gfx3d = b.addStaticLibrary(.{
        .name = "groover-gfx3d",
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize
    });

    const exe = b.addExecutable(.{
        .name = "hello-world",
        .root_source_file = b.path("hello.zig"),
        .target = target,
        .optimize = optimize
    });

    exe.linkLibrary(lib_groove_gfx3d);

    b.installArtifact(exe);

}
