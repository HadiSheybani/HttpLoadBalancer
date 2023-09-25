

enable_testing()

set(TESTS_SOURCE_DIR src)

set(TESTS_SOURCE_CODES
    ${TESTS_SOURCE_DIR}/main.cpp
    ${TESTS_SOURCE_DIR}/test_SimpleBalancer.cpp
    ${TESTS_SOURCE_DIR}/test_SimpleBroker.cpp)

add_executable(${TESTS_TARGET_NAME} ${TESTS_SOURCE_CODES})

target_link_libraries(${TESTS_TARGET_NAME}
    GTest::gtest_main
    GTest::gmock_main
)

set_target_properties(${TESTS_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

install(TARGETS ${TESTS_TARGET_NAME}
    CONFIGURATIONS Debug Release
    RUNTIME DESTINATION bin)