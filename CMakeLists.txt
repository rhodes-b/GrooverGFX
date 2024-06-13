cmake_minimum_required(VERSION 3.22)
set(PROJECT groover-gfx)
project(${PROJECT})

set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT}
    main.c
)

target_link_libraries(${PROJECT}
    groover-image
    groover-painter
)

add_subdirectory(src)
# TODO:
# add_subdirectory(tests)