
add_library(groover-image STATIC
    image.c
)

target_include_directories(groover-image PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)