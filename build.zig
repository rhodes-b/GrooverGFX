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

    const groove_gfx = b.addModule("groove_gfx", .{
        .root_source_file = b.path("src/root.zig"),
        .target = target,
        .optimize = optimize,
    });

    const Example = enum {
        scene0,
        scene1,
        scene2,
        scene3,
        scene3a,
        scene3b,
        scene3c,
        scene3d,
        scene3e,
        scene4,
    };

    const run_example = b.option(Example, "example", "example scenes to run");
    const example_step = b.step("example", "run example");
    if (run_example) |example_to_run| {
        const example = b.addExecutable(.{
            .name = @tagName(example_to_run),
            .root_source_file = b.path(b.fmt("examples/{s}.zig", .{@tagName(example_to_run)})),
            .target = target,
            .optimize = optimize,
        });
        example.root_module.addImport("groove_gfx", groove_gfx);
        const example_run = b.addRunArtifact(example);
        example_step.dependOn(&example_run.step);
    }
}
