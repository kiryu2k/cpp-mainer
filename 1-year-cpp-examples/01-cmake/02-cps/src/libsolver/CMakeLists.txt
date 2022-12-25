set(target_name solver)

add_library(${target_name} STATIC
  libsolver/sqrt.cpp
  libsolver/sqrt.hpp)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
  ${target_name}
  PRIVATE
    m
)
