
add_subdirectory(mocks)

set(RYANMOCK_LIB
    ${CMAKE_BINARY_DIR}/ryanmock/src/ryanmock-build/libryanmock.a
)
set(RYANMOCK_INCLUDE_DIR
    ${CMAKE_BINARY_DIR}/ryanmock/src/ryanmock/include/
    ${CMAKE_BINARY_DIR}/ryanmock/src/ryanmock-build/
)

set(TESTS
    image
)

add_custom_target(run_tests)

foreach(test ${TESTS})
    add_executable(test_${test} test_${test}.c)

    target_include_directories(test_${test} PUBLIC
        ${RYANMOCK_INCLUDE_DIR}
    )

    target_link_libraries(test_${test} PUBLIC
        groover-image
        ${RYANMOCK_LIB}
    )

    add_dependencies(test_${test} ryanmock)

    add_custom_target(run_test_${test}
        COMMAND test_${test}
    )

    add_custom_command(TARGET run_test_${test}
        POST_BUILD
        COMMAND test_${test}
        VERBATIM
    )

    add_dependencies(run_tests run_test_${test})

endforeach()
