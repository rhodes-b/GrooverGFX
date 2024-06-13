
add_library(groover-image STATIC
    image.c
)

add_library(groover-painter STATIC
    painter.c
)

target_include_directories(groover-image PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_include_directories(groover-painter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(groover-painter PUBLIC
    groover-image
    m
)