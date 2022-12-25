set(target_name geometry-c)

add_library(${target_name} SHARED
  libgeometry-c/geometry.h
  libgeometry-c/Circle.cpp
  libgeometry-c/Circle.h
  libgeometry-c/Point.h
  libgeometry-c/Shape.cpp
  libgeometry-c/Shape.h
)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${target_name} PUBLIC geometry)
