
add_library(groover-render3d STATIC
    camera.c
    image.c
    math3d.c
    matrix.c
    models.c
    ray3d.c
    render_oo.c
    render_ray.c
    rgb.c
    scenedef.c
)

target_include_directories(groover-render3d PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)