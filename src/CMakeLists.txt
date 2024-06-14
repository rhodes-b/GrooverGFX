
add_library(groover-image STATIC
    image.c
)

add_library(groover-painter STATIC
    painter.c
)

add_library(groover-matrix STATIC
    matrix.c
)

target_include_directories(groover-image PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_include_directories(groover-painter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_include_directories(groover-matrix PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(groover-painter PUBLIC
    groover-image
    m
)

target_link_directories(groover-matrix PUBLIC
    m
)
