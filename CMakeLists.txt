# add src directory
add_subdirectory(src)

# TODO:
# add_subdirectory(tests)

add_executable(groover-gfx
    main.c
)

target_link_libraries(groover-gfx
    groover-image
)