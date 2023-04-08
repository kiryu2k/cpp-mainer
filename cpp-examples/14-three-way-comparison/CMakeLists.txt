cmake_minimum_required(VERSION 3.22)

project(ThreeWayComparison)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

add_subdirectory(external)

include(CompileOptions)

file(GLOB source_paths "src/*.cpp")
foreach(source_path ${source_paths})
  get_filename_component(source_file_name ${source_path} NAME_WE)
  
  add_executable(${source_file_name} ${source_path})
  set_compile_options(${source_file_name})
  target_link_libraries(${source_file_name} PRIVATE gtest gtest_main)
endforeach()
