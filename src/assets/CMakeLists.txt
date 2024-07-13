
set(PPM_FILES
    dino
    wartburg
)

foreach(file ${PPM_FILES})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${file}.ppm DESTINATION ${CMAKE_BINARY_DIR}/ppm/)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${file}.ppm DESTINATION ${CMAKE_BINARY_DIR}/tests/ppm/)
endforeach()
