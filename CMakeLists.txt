cmake_minimum_required(VERSION 3.24)
project(somos)

set(CMAKE_CXX_STANDARD 17)
set(SOMOS_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SOMOS_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(SOMOS_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_roms DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests)

include_directories(${SOMOS_INSTALL_INCLUDE_DIR})
include_directories(${SOMOS_INSTALL_LIB_DIR})

add_subdirectory(src)
add_subdirectory(vendor)
add_subdirectory(tests)
