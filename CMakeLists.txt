cmake_minimum_required(VERSION 3.16)

project(HttpLoadBalancer VERSION "1.0.0.0")
set(TESTS_TARGET_NAME ${PROJECT_NAME}Tests)

option(BUILD_WITH_TESTS "BUILD_WITH_TESTS" ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

include_directories(include)

if (BUILD_WITH_TESTS)
    include(add_gtest)

    add_subdirectory(tests)
endif()