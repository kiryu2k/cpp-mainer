cmake_minimum_required(VERSION 3.22)

project(HelloWorld)

add_executable(hello src/main.cpp)

set_target_properties(
  hello
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

if(MSVC)
  target_compile_options(hello PRIVATE /W4 /WX)
else()
  target_compile_options(
    hello
    PRIVATE
      -Wall
      -Wextra
      -Werror
      -pedantic)
endif()

