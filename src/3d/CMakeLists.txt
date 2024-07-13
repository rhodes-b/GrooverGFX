
add_library(groover-render3d STATIC
    camera.c
    image.c
    linked_list.c
    math3d.c
    matrix.c
    models.c
    ray3d.c
    render_oo.c
    render_ray.c
    scenedef.c
)

target_include_directories(groover-render3d PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(groover-render3d PUBLIC
    m
)