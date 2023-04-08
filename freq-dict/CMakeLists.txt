cmake_minimum_required(VERSION 3.10)
project(my_app_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(abseil-cpp)

add_executable(main main.cpp robin_hood.h)
target_link_libraries(main absl::flat_hash_map)