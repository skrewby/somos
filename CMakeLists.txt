cmake_minimum_required(VERSION 3.24)
project(somos)

set(CMAKE_CXX_STANDARD 17)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(vendor)