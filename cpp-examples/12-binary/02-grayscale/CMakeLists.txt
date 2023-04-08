cmake_minimum_required(VERSION 3.21)

project(Grayscale)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(CompileOptions)

add_subdirectory(external)
add_subdirectory(src)
