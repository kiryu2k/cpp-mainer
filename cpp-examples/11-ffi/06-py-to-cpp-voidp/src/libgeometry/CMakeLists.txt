set(target_name geometry)

add_library(${target_name} STATIC
    libgeometry/Circle.cpp
    libgeometry/Circle.hpp
    libgeometry/Point.hpp
    libgeometry/Shape.hpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
