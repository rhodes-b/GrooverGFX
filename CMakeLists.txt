cmake_minimum_required(VERSION 3.22)
set(PROJECT groover-gfx)
project(${PROJECT})

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)
add_subdirectory(tests)

set(SCENES
    scene0
    scene1
)

add_executable(2d-lib
    main.c
)

foreach(scene ${SCENES})
    add_executable(${scene}
        ${scene}.c
    )

    target_link_libraries(${scene}
        groover-render3d
    )

endforeach()


target_link_libraries(2d-lib
    groover-render2d
)

# Install Testing Framework
include(ExternalProject)
ExternalProject_Add(
    ryanmock
    PREFIX ryanmock
    GIT_REPOSITORY https://gitlab.com/hansonry/ryanmock.git
    GIT_TAG v1.6.2
    CMAKE_ARGS -DRYANMOCK_BUILD_EXAMPLE:BOOL=OFF -DRYANMOCK_RUN_TESTS:BOOL=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ryanmock
    INSTALL_COMMAND ${CMAKE_COMMAND} --build .
)