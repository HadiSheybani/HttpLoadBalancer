cmake_minimum_required(VERSION 3.16)

project(HttpLoadBalancer VERSION "1.0.0.0")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
set(CMAKE_DEBUG_POSTFIX d)

include_directories(include)